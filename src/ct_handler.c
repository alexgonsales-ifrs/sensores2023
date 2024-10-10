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
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variaveis Públicas ===================================================

//uint8_t hand_rcif;

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
 */
void __interrupt() handler(void) {
  TBotao botao;
  static uint16_t static_count_t0 = 0;
  
  //Testa para ver qual o tipo de interrupção que ocorreu:

  //==================== Testa interrupção Porta Serial ===============
  if (PIE1bits.RCIE) {
    if (PIR1bits.RCIF) {
      //hand_rcif = 1;
      prot_rs232_executa();
      
      /*
      if (RCREG == 0x41) { //letra 'A'
        serv_rs232_envia_leituras_gravadas_eeprom();
        //RCIF é somente leitura. É zerada quando se le o RCREG:
        //PIR1bits.RCIF = 0;
      }
      */
      
      //RCIF é somente leitura. É zerada quando se le o RCREG:
      //PIR1bits.RCIF = 0;
    }
  }//if (PIE1bits.RCIE) - interrupção Porta Serial.

  //================== Testa interrupção do Timer0 ========================
  if (INTCONbits.T0IE) {
    if (INTCONbits.T0IF) {

      //Se recem ligou o equipamento, então chama est_maquina() para fazer inicializações.
      if (est_estado_atual == EST_ESTADO_NULL) {
        est_maquina(BTN_NULL);
      }
      
      #ifdef _HARDWARE_2013_
      //Trata interface dos botões.
      //Só vai funcionar se o Timer0 estiver ligado, não deveria ser assim.
      TBotao botao = btns_testa_antigo();
      if (botao != 0) {
        est_maquina(botao);
      }
      #endif

      //Trata leitura sensores.
      else if (  (est_estado_atual==EST_ESTADO_MONITORA) ||  (est_estado_atual==EST_ESTADO_MONITORA_GRAVA)  ) {
        //Verifica no timer0 se já passou a contagem de tempo para efetuar uma amostra.
        if (serv_adcon_testa_timer_tempo_aquisicao(static_count_t0)) {
          //Ja passou a contagem do Timer0, então efetua uma amostra e zera a contagem.
          if (est_estado_atual == EST_ESTADO_MONITORA) {
            //serv_adcon_aquisicao_print();
            serv_adcon_aquisicao();
            serv_adcon_print();
          } else if (est_estado_atual==EST_ESTADO_MONITORA_GRAVA ) {
            //Se o módulo serv_adcon está monitorando e gravando então
            if (serv_adcon_monitora_grava) {
              serv_adcon_aquisicao_print_grava();
            }
            else {
              //Módulo sinalizou que interrompeu a gravação, então deveria avisar máquina de estados e não atribuir diretamente, verificar<<<<<<<<<<<<<<<<<<
              //<<<<<<<<<<<<<<<<<<<
              //serv_adcon_monitora_grava = 0;  //<<<<<<<< função serv_adcon_aquisicao_print_grava() já fez isso. retirado em 2024-08-03 alexdg)
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
  }//if (INTCONbits.T0IE) interrupção Timer0
  
  //==================== Testa interrupção PortB (botoes) ==================
  
  #ifdef _HARDWARE_2016_

  if (INTCONbits.RBIE) {
    if (INTCONbits.RBIF) {
      botao = btns_testa();
      if (botao != 0) {
        est_maquina(botao);
      }
      PORTB = PORTB; //para poder limpar o RBIF.
      INTCONbits.RBIF = 0;
      }//if (INTCONbits.RBIF)
  }//if (INTCONbits.RBIE) - interrupção PortB.
  
  #endif //_HARDWARE_2016_
    
  
}//handler())

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
