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
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================
#define HAND_TAM_BUFFER_RS232 8

//===== Variaveis P�blicas ===================================================

volatile uint8_t hand_flag_botao  = 0;
volatile uint8_t hand_flag_timer0 = 0;
volatile uint8_t hand_flag_rs232  = 0;

volatile uint8_t hand_vbuffer_rs232[HAND_TAM_BUFFER_RS232];
volatile uint8_t hand_qt_buffer_rs232=0;

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variaveis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

//static uint8_t hand_adcon_monitora;

/**
 * Funcao que trata as interrup��es.
 * 
 * Nao desabilitar a interrupcao Global INTCONbits.GIE.
 * Ela � desabilitada automaticamente quando inicia a rotina de tratamento da interrup��o e 
 * habilitada automaticamente quando a rotina de tratamento de interrup��o � concluida.
 * 
 * Nao habilitar INTCONbits.GIE pois a instrucao RETFIE ja faz isso.
 * Item 5.8.5, pg. 130 User Guide XC8 (50002737):
 * Never re-enable interrupts inside the interrupt function itself. 
 * Interrupts are automatically re-enabled by hardware on execution of the
 * RETFIE instruction. Re-enabling interrupts inside an interrupt function
 * may result in code failure.
 * 
 * O par�grafo acima parece se referir somente � GIE global e n�o aos bits individuais.
 *
 * Se��o 14.3 (Interrupts) do datasheet do PIC (DS40001291H):
 * The interrupt flag bit(s) must be cleared in software before
 * re-enabling interrupts to avoid multiple interrupt
 * requests.
 * 
 * No caso da interrup��o da porta serial, o bit PIR1bits.RCIF n�o deve ser atualizado
 * 
 * Os comandos recebidos pela porta serial devem terminar com o caractere <LF> (0x0A).
 * Os demais caracteres de controle, abaixo de 32 (0x20), ser�o desconsiderados (descartados).
 * A vari�vel hand_vbuffer_rs232[] conter� a string (comando) recebida pela serial,
 * sem o caractere <LF> e terminada com o caractere nulo (\0).
 * 
 * Todas as strings que o equipamento envia pela serial tamb�m s�o finalizados
 * com um final de linha (\n).
 * 
 * Nota: \n e <LF> s�o equivalentes.
 * 
 */
void __interrupt() handler(void) {
  //Testa para ver qual o tipo de interrup��o que ocorreu:

  //==================== Testa interrup��o Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      uint8_t c; //c � o caractere (byte) lido da porta serial.
      //Faz a leitura do byte (RCREG) que est� na porta serial, assim, a flag RCIF ser� zerada.
      c = RCREG;
      
      //Se houver espa�o no buffer ent�o
      if (hand_qt_buffer_rs232 < HAND_TAM_BUFFER_RS232) {
        //Se chegou o caractere LF ent�o finaliza a string (comando).
        if (c == 0x0A) {
            //Liga a flag para indicar para o main() tratar a linha recebida.
            hand_flag_rs232 = 1;
            //Desabilita interrup��o RX at� que o buffer seja tratado no main().
            //Ser� habilitado novamente no main(), ap�s tratar o dado.
            PIE1bits.RCIE = 0;
            //Coloca um caractere nulo (\0) no buffer para indicar final de string.
            hand_vbuffer_rs232[hand_qt_buffer_rs232] = '\0';
            hand_qt_buffer_rs232++;
        }//LF
        else if (c >= 32) {
          //� um caractere a partir de 0x20 (32), ent�o
          //adiciona o caractere no buffer (caracteres abaixo de 32, com exce��o do LF, s�o desconsiderados).
          //Observe que o caractere LF j� foi tratado no if anterior.
          hand_vbuffer_rs232[hand_qt_buffer_rs232] = c;
          hand_qt_buffer_rs232++;
        }//c>=32
      }//hand_qt_buffer_rs232< HAND_TAM_BUFFER_RS232
      else { //N�o tem espa�o no buffer 
        //Buffer overflow, ent�o limpa o buffer, desconsiderando o que foi recebido.
        hand_qt_buffer_rs232 = 0;
      }
      //RCIF � read-only, sendo zerada automaticamente quando se le o RCREG.
      //Portanto, n�o pode fazer a linha abaixo:
      //PIR1bits.RCIF = 0;
      
    }//if (PIR1bits.RCIF)
  }//if (PIE1bits.RCIE) 

  //================== Testa interrup��o do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {
      //Liga a flag para indicar para o main() a interrup��o que ocorreu.
      hand_flag_timer0 = 1;

      TMR0 = 39;           //Tem que recarrecar o valor no TMR0.
      INTCONbits.T0IF = 0; //Tem que zerar T0IF em software AQUI.

      //Por quest�o de exatid�o do timer, talvez seja necess�rio desabilitar T0IE
      //e s� reabilitar depois que a fun��o foi tratada no main().
      //Se isso for feito, n�o esquecer de fazer TMR0=39 l� tamb�m.
      //INTCONbits.T0IE = 0;
    }//if (INTCONbits.T0IF)
  }//if (INTCONbits.T0IE) interrup��o Timer0
  
  //==================== Testa interrup��o PortB (botoes) ==================
  #ifdef _HARDWARE_2016_
  if (INTCONbits.RBIE) {
    if (INTCONbits.RBIF) {
      //Liga a flag para indicar para o main() a interrup��o que ocorreu.
      hand_flag_botao = 1;
      
      PORTB = PORTB;       //para poder limpar o RBIF.
      INTCONbits.RBIF = 0; //Tem que zerar RBIF em software AQUI.
    }//if (INTCONbits.RBIF)
  }//if (INTCONbits.RBIE) - interrup��o PortB.
  #endif //_HARDWARE_2016_
  
}//handler())

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
