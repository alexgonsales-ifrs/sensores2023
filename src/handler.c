#include <xc.h>
#include <stdint.h>

#include "handler.h"
#include "versao.h"

#include "botoes.h"
#include "estados.h"
#include "serv_adcon.h"
//#include "serv_rs232.h"
#include "prot_rs232.h"

/**
 * Funcao que trata as interrupções.
 */
void __interrupt() handler(void) {
  TBotao botao;
  static uint16_t static_count_t0 = 0;
  
  //Nao precisa desabilitar a interrupcao Global.
  //Ela é desabilitada automaticamente quando inicia a rotina d tratamento da interrupção e 
  //habilitada automaticamente quando a rotina de tratamento de interrupção é concluida.
    
  //Testa para ver qual o tipo de interrupção que ocorreu:

  //==================== Testa interrupção Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      prot_rs232_executa();
      /*
      if (RCREG == 0x41) { //letra 'A'
        serv_rs232_envia_leituras_gravadas_eeprom();
        //RCIF é somente leitura. É zerada quando se le o RCREG:
        //PIR1bits.RCIF = 0;
      }
      */
    }
  }//if (PIE1bits.RCIE) - interrupção Porta Serial.

  //================== Testa interrupção do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {
      //Verifica no timer0 se já passou a contagem de tempo para efetuar uma amostra.
      if (serv_adcon_testa_timer_tempo_amostra(static_count_t0)) {
        //Ja passou a contagem do Timer0, então efetua uma amostra e zera a contagem.
        if (est_estado_atual == EST_ESTADO_MONITORA) {
          serv_adcon_amostra_print();
        } else if (est_estado_atual == EST_ESTADO_MONITORA_GRAVA) {
          serv_adcon_amostra_print_grava();
        }
        static_count_t0 = 0;
      } 
      else {
        static_count_t0++;
      }//else
      TMR0 = 39; //para dar overflow antes de 256 ints
      //T0IF tem que ser zerado em software.
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
