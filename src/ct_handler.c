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
 * Os comandos recebidos pela porta serial devem terminar com um ponto (".") e 
 * não devem conter nenhum caractere de final de linha (CR LF).
 * 
 */
void __interrupt() handler(void) {
  //Testa para ver qual o tipo de interrupção que ocorreu:

  //==================== Testa interrupção Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      //Se houver espaço no buffer então
      if (hand_qt_buffer_rs232 < HAND_TAM_BUFFER_RS232) {
        //Le o caractere da porta serial e adiciona no buffer.
        uint8_t c;
        //Faz a leitura do byte (RCREG) que está na porta serial, assim, a flag RCIF será zerada.
        c = RCREG;        
        //Adiciona o byte no buffer (caracteres abaixo de 32 serão desconsiderados).
        if (c >= 32) {
          hand_vbuffer_rs232[hand_qt_buffer_rs232] = c;
          hand_qt_buffer_rs232++;
          //Testa final de linha recebido na serial.
          if (c == '.') {
            //Liga a flag para indicar para o main() tratar a linha recebida.
            hand_flag_rs232 = 1;
            //Desabilita interrupção RX até que o buffer seja tratado no main().
            //Será habilitado novamente no main(), após tratar o dado.
            PIE1bits.RCIE = 0;
          }//c=='.'
        }//c>=32
      }//hand_qt_buffer_rs232< HAND_TAM_BUFFER_RS232
      else { 
        //Buffer overflow, então limpa o buffer, desconsiderando o que foi recebido.
        hand_qt_buffer_rs232 = 0;
      }
      
      //RCIF é read-only. É zerada automaticamente quando se le o RCREG.
      //Portanto, não pode fazer a linha abaixo:
      //PIR1bits.RCIF = 0;
    }
  }//if (PIE1bits.RCIE) - interrupção Porta Serial.

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
