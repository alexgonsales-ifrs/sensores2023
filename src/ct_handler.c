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

#include "base_botoes.h"
#include "ct_estados.h"
#include "serv_adcon.h"
#include "ct_prot_rs232.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Variaveis P�blicas ===================================================

//uint8_t hand_rcif;

//============================================================================
//===== Defini��es Privadas ==================================================
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
 */

void __interrupt() handler(void) {
  TBotao botao;
  static uint16_t static_count_t0 = 0;
  
  //Nao precisa desabilitar a interrupcao Global.
  //Ela � desabilitada automaticamente quando inicia a rotina d tratamento da interrup��o e 
  //habilitada automaticamente quando a rotina de tratamento de interrup��o � concluida.
    
  //Testa para ver qual o tipo de interrup��o que ocorreu:

  //==================== Testa interrup��o Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      //hand_rcif = 1;
      prot_rs232_executa();
      
      /*
      if (RCREG == 0x41) { //letra 'A'
        serv_rs232_envia_leituras_gravadas_eeprom();
        //RCIF � somente leitura. � zerada quando se le o RCREG:
        //PIR1bits.RCIF = 0;
      }
      */
    }
  }//if (PIE1bits.RCIE) - interrup��o Porta Serial.

  //================== Testa interrup��o do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {

      //Se recem ligou o equipamento, ent�o chama est_maquina() para fazer inicializa��es.
      if (est_estado_atual == EST_ESTADO_NULL) {
        est_maquina(BTN_NULL);
      }
      
      #ifdef _HARDWARE_ANTIGO_
      //Trata interface dos bot�es.
      //S� vai funcionar se o Timer0 estiver ligado, n�o deveria ser assim.
      TBotao botao = btns_testa_antigo();
      if (botao != 0) {
        est_maquina(botao);
      }
      #endif

      //Trata leitura sensores.
      else if (  (est_estado_atual==EST_ESTADO_MONITORA) ||  (est_estado_atual==EST_ESTADO_MONITORA_GRAVA)  ) {
        //Verifica no timer0 se j� passou a contagem de tempo para efetuar uma amostra.
        if (serv_adcon_testa_timer_tempo_aquisicao(static_count_t0)) {
          //Ja passou a contagem do Timer0, ent�o efetua uma amostra e zera a contagem.
          if (est_estado_atual == EST_ESTADO_MONITORA) {
            serv_adcon_aquisicao_print();
          } else if (est_estado_atual==EST_ESTADO_MONITORA_GRAVA ) {
            //Se o m�dulo serv_adcon est� monitorando e gravando ent�o
            if (serv_adcon_monitora_grava) {
              serv_adcon_aquisicao_print_grava();
            }
            else {
              //M�dulo sinalizou que interrompeu a grava��o, ent�o deveria avisar m�quina de estados e n�o atribuir diretamente, verificar<<<<<<<<<<<<<<<<<<
              //<<<<<<<<<<<<<<<<<<<
              //serv_adcon_monitora_grava = 0;  //<<<<<<<< fun��o serv_adcon_aquisicao_print_grava() j� fez isso. retirado em 2024-08-03 alexdg)
            }
          }
          static_count_t0 = 0;
        } 
        else {
          static_count_t0++;
        }//else
      }//if est_estado_atual

      
      TMR0 = 39; //para dar overflow antes de 256 ints
      //T0IF tem que ser zerado em software.
      INTCONbits.T0IF = 0;
      
    }//if (INTCONbits.T0IF)
  }//if (INTCONbits.T0IE) interrup��o Timer0
  
  //==================== Testa interrup��o PortB (botoes) ==================
  #ifdef _HARDWARE_NOVO_
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
  }//if (RBIE) - interrup��o PortB.
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

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
