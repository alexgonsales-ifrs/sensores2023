#include <xc.h> //
#include "xtal.h"

#include "prot_rs232.h"
#include "serv_rs232.h"
#include "estados.h"
#include "rs232.h"
#include "adcon.h"
#include "eeprom.h"

/*****************************************************************************
 * File:   prot_rs232.c
 * Author: Alex
 * Comments:
 * 
 * Revision history: 
 *
 ****************************************************************************/

//===== Includes =============================================================

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variáveis Públicas ===================================================

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variáveis Privadas ===================================================

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

void prot_rs232_executa() {
  //Melhorar esta lógica para funcionar em qualquer ESTADO.
  
  uint8_t rx = RCREG;
  if (est_estado_atual != EST_ESTADO_MONITORA &&
      est_estado_atual != EST_ESTADO_MONITORA_GRAVA &&
      est_estado_atual != EST_ESTADO_ENVIAR_DADOS) {
      if (rx == 0x41) { //letra 'A'
        serv_rs232_envia_leituras_gravadas_eeprom();
      }
      else if (rx == 0x01) {
        //Entra no estado Monitora.
        est_estado_atual = EST_ESTADO_MONITORA;
        //Habilita monitora.
        INTCONbits.T0IE = 1;
      }
      else if (rx == 0x02) {
        //Entra no estado Monitora e Grava.
        est_estado_atual = EST_ESTADO_MONITORA_GRAVA;
        //Habilita monitora.
        INTCONbits.T0IE = 1;
      }
      else if (rx == 0x04) {
        //Envia leituras armazenadas na EEPROM.
        serv_rs232_envia_leituras_gravadas_eeprom();
      }
      else if (rx == 0x05) {
        //Envia Quant. Sensores.
        rs232_envia_byte(adcon_cfg_quant_sensores_atual);
      }
      else if (rx == 0x06) {
        //Envia Tempo Amostra.
        //Envia byte mais significativo.
        rs232_envia_byte((uint8_t)(adcon_cfg_tempo_amostra_atual >> 8));
        //Envia byte menos significativo.
        rs232_envia_byte((uint8_t)adcon_cfg_tempo_amostra_atual);
      }
      else if (rx == 0x07) {
        //Grava Quant. Sensores.
        rx = RCREG;
        eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, rx);
      }
      else if (rx == 0x08) {
        //Grava Tempo Amostra.
        uint16_t tempo_amostra;
        //Byte mais significativo;
        rx = RCREG;        
        tempo_amostra = rx;
        tempo_amostra = tempo_amostra << 8;
        __delay_ms(50);
        if (RCIF == 1) {
          //Byte menos significativo;
          rx = RCREG;
          tempo_amostra = tempo_amostra + rx;
          eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, tempo_amostra);
        }
      }
  }//if

  if (est_estado_atual == EST_ESTADO_MONITORA ||
      est_estado_atual == EST_ESTADO_MONITORA_GRAVA ||
      est_estado_atual == EST_ESTADO_ENVIAR_DADOS) {
      if (rx == 0x03) {
        //Para monitoramento.
        est_estado_atual = EST_ESTADO_MENU_PRINCIPAL;
        //Desabilita monitora.
        INTCONbits.T0IE = 0;
      }
  }//if

  
}//prot_rs232_executa()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

