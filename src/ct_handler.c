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

#include "base_botoes.h"  //<<<<< j� incluido no header.

/*
#include "base_botoes.h"
#include "ct_estados.h"
#include "serv_adcon.h"
#include "ct_prot_rs232.h"
#include "serv_dht22.h"
*/

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Variaveis P�blicas ===================================================

volatile uint8_t hand_flag;
volatile TBotao  hand_botao_pressionado = BTN_NULL;

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
 */
void __interrupt() handler(void) {
  //TBotao botao;
  //static uint16_t static_count_t0 = 0;
  
  //Testa para ver qual o tipo de interrup��o que ocorreu:

  //==================== Testa interrup��o Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      //hand_rcif = 1;
      
      //<<<<<<<<<<<<<<<< Desabilitado em 22/04/2025 pois est� acontecendo
      //um erro estranho (aleat�rio) em determinados submenus.
      //Talvez algum acesso concorrente � mesma �rea de mem�ria.
      //prot_rs232_executa();
      
      /*
      if (RCREG == 0x41) { //letra 'A'
        serv_rs232_envia_leituras_gravadas_eeprom();
        //RCIF � somente leitura. � zerada quando se le o RCREG:
        //PIR1bits.RCIF = 0;
      }
      */
      
      //RCIF � somente leitura. � zerada quando se le o RCREG:
      //PIR1bits.RCIF = 0;
    }
  }//if (PIE1bits.RCIE) - interrup��o Porta Serial.

  //================== Testa interrup��o do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {

      /*
       
      //Se recem ligou o equipamento, ent�o chama est_maquina() para fazer inicializa��es.
      if (est_estado_atual == EST_ESTADO_NULL) {
        est_maquina(BTN_NULL);
      }
      
      #ifdef _HARDWARE_2013_
      //Trata interface dos bot�es.
      //S� vai funcionar se o Timer0 estiver ligado, n�o deveria ser assim.
      TBotao botao = btns_testa_antigo();
      if (botao != 0) {
        est_maquina(botao);
      }
      #endif

      //Trata leitura sensores.
      else if (  (est_estado_atual==EST_ESTADO_MONITORA) ||  (est_estado_atual==EST_ESTADO_MONITORA_GRAVA)  ) {
        uint16_t tempo;
        //Verifica no timer0 se j� passou a contagem de tempo para efetuar uma amostra.
        tempo = serv_adcon_testa_timer_tempo_aquisicao(static_count_t0);
        if (tempo) {
          //Ja passou a contagem do Timer0, ent�o efetua uma amostra e zera a contagem.
          if (est_estado_atual == EST_ESTADO_MONITORA) {
            serv_adcon_aquisicao();
            serv_adcon_print();
            serv_dht22_amostra_e_print();
            //N�o precisa (nem pode) desabilitar a interrup��o global aqui
            //pois o pr�prio handler j� desabilita automaticamente.
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
      */
      
      hand_flag = 2;
      
    }//if (INTCONbits.T0IF)
  }//if (INTCONbits.T0IE) interrup��o Timer0
  
  //==================== Testa interrup��o PortB (botoes) ==================
  
  #ifdef _HARDWARE_2016_

  if (INTCONbits.RBIE) {
    if (INTCONbits.RBIF) {

      //Considera apenas o pressionamento do botao,
      //desconsiderando o "soltar" do bot�o.
      TBotao btn = btns_testa();
      if (btn != BTN_NULL) {
        hand_flag = 1;
        hand_botao_pressionado = btn;
      }
      PORTB = PORTB; //para poder limpar o RBIF.
      INTCONbits.RBIF = 0;      
      
      /*
      if (botao != 0) {
        est_maquina(botao);
      }
      PORTB = PORTB; //para poder limpar o RBIF.
      INTCONbits.RBIF = 0;
       */
      
      }//if (INTCONbits.RBIF)
  }//if (INTCONbits.RBIE) - interrup��o PortB.
  
  #endif //_HARDWARE_2016_
    
  
}//handler())

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
