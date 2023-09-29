#include <xc.h>
#include <stdint.h>

#include "handler.h"
#include "adcon.h"
#include "botoes.h"
#include "eeprom.h"
#include "rs232.h"
#include "versao.h"

#include "estados.h"
#include "cfg_tempo_amostra.h"

/**
 * Funcao que trata as interrupções.
 */
void __interrupt() handler(void) {
  TBotao botao;
  static uint16_t count_t0 = 0;
  uint16_t tempo_amostra;

  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  //Ver com Tiago ou Felipe por que comentaram este código.
  //Nao precisa desabilitar a interrupcao Global???

  //INTCONbits.GIE = 0; //desabilita interrupcao global
    
  //Testa para ver qual o tipo de interrupção que ocorreu:

  //==================== Testa interrupção Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      if (RCREG == 0x41) { //letra 'A'
        //if (RCREG == 'A') { //letra 'A'
        //lcd_clear();
        //static int clr = 0;
        //if (clr == 0) {
        //    lcd_clear();
        //    clr = 1;
        //}

        //char tmp[];
        //sprintf(tmp, "%c bytes transmitidos", rs232_transmite());
        //lcd_puts(tmp);
        rs232_transmite();

        //RCIF e zerada quando se le o RCREG:
        //PIR1bits.RCIF = 0;
      }
    }
  }//if (PIE1bits.RCIE) - interrupção Porta Serial.

  //================== Testa interrupção do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {
      //Verifica no timer0 se já passou a contagem de tempo para efetuar uma amostra.
      //tempo_amostra = menu_get_quant(&amo_menu_tempo_amostra);
      tempo_amostra = cfg_tempo_amostra_atual;
      if (count_t0 >= tempo_amostra) {
        //Ja passou a contagem do Timer0, então efetua uma amostra e zera a contagem.
        if (est_estado_atual == EST_ESTADO_MONITORA) {
          adcon_amostra_print();
        } else if (est_estado_atual == EST_ESTADO_MONITORA_GRAVA) {
          adcon_amostra_print_grava();
        }
        count_t0 = 0;
      } 
      else {
        count_t0++;
      }//else
      TMR0 = 39; //para dar overflow antes de 256 ints
      INTCONbits.T0IF = 0;
    }//if (INTCONbits.T0IF)
  }//if (INTCONbits.T0IE) interrupção Timer0
  
  //==================== Testa interrupção PortB (botoes) ==================
  #ifdef _MODULO_NOVO_
  //if (INTCONbits.RBIE) {
  if (RBIE) {
    //if (INTCONbits.RBIF) {
    if (RBIF) {
      //guia do xc8 especifica para nao reabilitar interrupcoes dentro de handlers:
      //INTCONbits.RBIE = 0;
        
      botao = btns_testa();

      if (botao != 0) {
        est_maquina(botao);
      }

      //INTCONbits.RBIF = 0;
      RBIF = 0;

      //guia do xc8 especifica para nao reabilitar interrupcoes dentro de handlers:
      //INTCONbits.RBIE = 1;
    }
  }//if (RBIE) - interrupção PortB.
  #endif //_MODULO_NOVO_
    
  /**
  * INTCONbits.GIE = 1; //habilita interrupcao global\n
  * Nao fazer isso pois a instrucao RETFIE ja faz isso.
  * Item 5.9.4, pg. 192 User Guide XC8\n
  * Never re-enable interrupts inside the interrupt function itself. 
  * Interrupts are automatically re-enabled by hardware on execution of the
  * RETFIE instruction. Re-enabling interrupts inside an interrupt function
  * may result in code failure.
  */
  
}//handler())
