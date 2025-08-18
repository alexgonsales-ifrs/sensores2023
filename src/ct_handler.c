/*****************************************************************************
 * File:   ct_handler.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <xc.h>
#include <stdint.h>

#include "ct_handler.h"
#include "versao.h"

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================
#define HAND_TAM_BUFFER_RS232 8

//===== Variaveis Públicas ===================================================

volatile uint8_t hand_flag_botao  = 0;
volatile uint8_t hand_flag_timer0 = 0;
volatile uint8_t hand_flag_rs232  = 0;

volatile uint8_t hand_vbuffer_rs232[HAND_TAM_BUFFER_RS232];
volatile uint8_t hand_qt_buffer_rs232=0;

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variaveis Privadas ===================================================

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

//static uint8_t hand_adcon_monitora;

/**
 * Funcao que trata as interrupções.
 * 
 * Nao desabilitar a interrupcao Global INTCONbits.GIE.
 * Ela é desabilitada automaticamente quando inicia a rotina de tratamento da interrupção e 
 * habilitada automaticamente quando a rotina de tratamento de interrupção é concluida.
 * 
 * Nao habilitar INTCONbits.GIE pois a instrucao RETFIE ja faz isso.
 * Item 5.8.5, pg. 130 User Guide XC8 (50002737):
 * Never re-enable interrupts inside the interrupt function itself. 
 * Interrupts are automatically re-enabled by hardware on execution of the
 * RETFIE instruction. Re-enabling interrupts inside an interrupt function
 * may result in code failure.
 * 
 * O parágrafo acima parece se referir somente à GIE global e não aos bits individuais.
 *
 * Seção 14.3 (Interrupts) do datasheet do PIC (DS40001291H):
 * The interrupt flag bit(s) must be cleared in software before
 * re-enabling interrupts to avoid multiple interrupt
 * requests.
 * 
 * No caso da interrupção da porta serial, o bit PIR1bits.RCIF não deve ser atualizado
 * 
 * Os comandos recebidos pela porta serial devem terminar com o caractere <LF> (0x0A).
 * Os demais caracteres de controle, abaixo de 32 (0x20), serão desconsiderados (descartados).
 * A variável hand_vbuffer_rs232[] conterá a string (comando) recebida pela serial,
 * sem o caractere <LF> e terminada com o caractere nulo (\0).
 * 
 * Todas as strings que o equipamento envia pela serial também são finalizados
 * com um final de linha (\n).
 * 
 * Nota: \n e <LF> são equivalentes.
 * 
 */
void __interrupt() handler(void) {
  //Testa para ver qual o tipo de interrupção que ocorreu:

  //==================== Testa interrupção Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      uint8_t c; //c é o caractere (byte) lido da porta serial.
      //Faz a leitura do byte (RCREG) que está na porta serial, assim, a flag RCIF será zerada.
      c = RCREG;
      
      //Se houver espaço no buffer então
      if (hand_qt_buffer_rs232 < HAND_TAM_BUFFER_RS232) {
        //Se chegou o caractere LF então finaliza a string (comando).
        if (c == 0x0A) {
            //Liga a flag para indicar para o main() tratar a linha recebida.
            hand_flag_rs232 = 1;
            //Desabilita interrupção RX até que o buffer seja tratado no main().
            //Será habilitado novamente no main(), após tratar o dado.
            PIE1bits.RCIE = 0;
            //Coloca um caractere nulo (\0) no buffer para indicar final de string.
            hand_vbuffer_rs232[hand_qt_buffer_rs232] = '\0';
            hand_qt_buffer_rs232++;
        }//LF
        else if (c >= 32) {
          //É um caractere a partir de 0x20 (32), então
          //adiciona o caractere no buffer (caracteres abaixo de 32, com exceção do LF, são desconsiderados).
          //Observe que o caractere LF já foi tratado no if anterior.
          hand_vbuffer_rs232[hand_qt_buffer_rs232] = c;
          hand_qt_buffer_rs232++;
        }//c>=32
      }//hand_qt_buffer_rs232< HAND_TAM_BUFFER_RS232
      else { //Não tem espaço no buffer 
        //Buffer overflow, então limpa o buffer, desconsiderando o que foi recebido.
        hand_qt_buffer_rs232 = 0;
      }
      //RCIF é read-only, sendo zerada automaticamente quando se le o RCREG.
      //Portanto, não pode fazer a linha abaixo:
      //PIR1bits.RCIF = 0;
      
    }//if (PIR1bits.RCIF)
  }//if (PIE1bits.RCIE) 

  //================== Testa interrupção do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {
      //Liga a flag para indicar para o main() a interrupção que ocorreu.
      hand_flag_timer0 = 1;

      TMR0 = 39;           //Tem que recarrecar o valor no TMR0.
      INTCONbits.T0IF = 0; //Tem que zerar T0IF em software AQUI.

      //Por questão de exatidão do timer, talvez seja necessário desabilitar T0IE
      //e só reabilitar depois que a função foi tratada no main().
      //Se isso for feito, não esquecer de fazer TMR0=39 lá também.
      //INTCONbits.T0IE = 0;
    }//if (INTCONbits.T0IF)
  }//if (INTCONbits.T0IE) interrupção Timer0
  
  //==================== Testa interrupção PortB (botoes) ==================
  #ifdef _HARDWARE_2016_
  if (INTCONbits.RBIE) {
    if (INTCONbits.RBIF) {
      //Liga a flag para indicar para o main() a interrupção que ocorreu.
      hand_flag_botao = 1;
      
      PORTB = PORTB;       //para poder limpar o RBIF.
      INTCONbits.RBIF = 0; //Tem que zerar RBIF em software AQUI.
    }//if (INTCONbits.RBIF)
  }//if (INTCONbits.RBIE) - interrupção PortB.
  #endif //_HARDWARE_2016_
  
}//handler())

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
