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
 * Os comandos recebidos pela porta serial devem terminar com um ponto (".") e 
 * n�o devem conter nenhum caractere de final de linha (CR LF).
 * 
 */
void __interrupt() handler(void) {
  //Testa para ver qual o tipo de interrup��o que ocorreu:

  //==================== Testa interrup��o Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      //Se houver espa�o no buffer ent�o
      if (hand_qt_buffer_rs232 < HAND_TAM_BUFFER_RS232) {
        //Le o caractere da porta serial e adiciona no buffer.
        uint8_t c;
        //Faz a leitura do byte (RCREG) que est� na porta serial, assim, a flag RCIF ser� zerada.
        c = RCREG;        
        //Adiciona o byte no buffer (caracteres abaixo de 32 ser�o desconsiderados).
        if (c >= 32) {
          hand_vbuffer_rs232[hand_qt_buffer_rs232] = c;
          hand_qt_buffer_rs232++;
          //Testa final de linha recebido na serial.
          if (c == '.') {
            //Liga a flag para indicar para o main() tratar a linha recebida.
            hand_flag_rs232 = 1;
            //Desabilita interrup��o RX at� que o buffer seja tratado no main().
            //Ser� habilitado novamente no main(), ap�s tratar o dado.
            PIE1bits.RCIE = 0;
          }//c=='.'
        }//c>=32
      }//hand_qt_buffer_rs232< HAND_TAM_BUFFER_RS232
      else { 
        //Buffer overflow, ent�o limpa o buffer, desconsiderando o que foi recebido.
        hand_qt_buffer_rs232 = 0;
      }
      
      //RCIF � read-only. � zerada automaticamente quando se le o RCREG.
      //Portanto, n�o pode fazer a linha abaixo:
      //PIR1bits.RCIF = 0;
    }
  }//if (PIE1bits.RCIE) - interrup��o Porta Serial.

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
