#include <xc.h> //
#include "xtal.h"

#include "string.h"

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

void prot_rs232_executa(void) {
  char tmp[8];
  //Melhorar esta lógica para funcionar em qualquer ESTADO.
  
  uint8_t rx;
  rx = RCREG;
  
  if (est_estado_atual != EST_ESTADO_MONITORA &&
      est_estado_atual != EST_ESTADO_MONITORA_GRAVA &&
      est_estado_atual != EST_ESTADO_ENVIAR_DADOS) {
    
      //Monitora (comando antigo).
      if (rx == 0x41) { //letra 'A'
        serv_rs232_envia_leituras_gravadas_eeprom();
      }//if 'A'
      
      //Monitora.
      else if (rx == 'B') {
        rs232_envia_string("Z\n");
        //TXREG = 'Z';
        //__delay_ms(5);
        //TXREG = 0x0D; //CR
        //__delay_ms(5);
        //TXREG = 0x0A; //LF
        //__delay_ms(5);
        
        sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);
        /*
        //S=2
        TXREG = 'S';
        __delay_ms(5);
        TXREG = '=';
        __delay_ms(5);
        TXREG = adcon_cfg_quant_sensores_atual + 48; //48 é o código ASCII do caractere zero (0).)
        __delay_ms(5);
        TXREG = 0x0D; //CR
        __delay_ms(5);
        TXREG = 0x0A; //LF
        __delay_ms(5);
         */

        sprintf(tmp, "T=%d\n", adcon_cfg_tempo_amostra_atual);
        rs232_envia_string(tmp);
        /*
        TXREG = 'T';
        __delay_ms(5);
        TXREG = '=';
        __delay_ms(5);        
        sprintf(tmp, "%d", adcon_cfg_tempo_amostra_atual/18);
        uint8_t i= 0;
        while (tmp[i]) {
          TXREG = tmp[i];
          __delay_ms(5);
          i++;
        }
        TXREG = 0x0D; //CR
        __delay_ms(5);
        TXREG = 0x0A; //LF
        __delay_ms(5);
         */

        //Entra no estado Monitora.
        est_estado_atual = EST_ESTADO_MONITORA;
        //Habilita monitora.
        INTCONbits.T0IE = 1;
      }//if 'B'
      
      //Monitora e Grava.
      else if (rx == 'C') {
        rs232_envia_string("Z\n");
        
        sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        sprintf(tmp, "T=%d\n", adcon_cfg_tempo_amostra_atual);
        rs232_envia_string(tmp);

        //Entra no estado Monitora e Grava.
        est_estado_atual = EST_ESTADO_MONITORA_GRAVA;
        //Habilita monitora.
        INTCONbits.T0IE = 1;
      }//if 'C'
      
      //Download. Equipamento envia dados armazenados na EEPROM.
      else if (rx == 'E') {
        rs232_envia_string("Z\n");
        
        sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        sprintf(tmp, "T=%d\n", adcon_cfg_tempo_amostra_atual);
        rs232_envia_string(tmp);
        
        sprintf(tmp, "Q=%d\n", adcon_quant_leituras_gravadas / adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        //Envia leituras armazenadas na EEPROM.
        serv_rs232_envia_leituras_gravadas_eeprom();
      }//if 'E'

      //LE_QUANT_SENSORES.
      else if (rx == 'F') {
        rs232_envia_string("Z\n");
        
        sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        //Envia Quant. Sensores.
        //rs232_envia_byte(adcon_cfg_quant_sensores_atual);
      }//if 'F'
      
      //LE_TEMPO_AMOSTRA
      else if (rx == 'G') {
        //Envia Tempo Amostra.
        rs232_envia_string("Z\n");
        
        sprintf(tmp, "T=%d\n", adcon_cfg_tempo_amostra_atual);
        rs232_envia_string(tmp);

        //Envia byte mais significativo.
        //rs232_envia_byte((uint8_t)(adcon_cfg_tempo_amostra_atual >> 8));
        //Envia byte menos significativo.
        //rs232_envia_byte((uint8_t)adcon_cfg_tempo_amostra_atual);
      }//if 'G'
      
      //Grava Quant Sensores.
      else if (rx == 'H') {
        while (!RCIF); //<<< loop para aguardar o próximo caractere.
        //Le a quant_sensores.
        rx = RCREG;
        rx = rx - 48; //48 é o código ASCII do caractere zero.
        //Grava quant_sensores na EEPROM.
        eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, rx);
        //Responde
        rs232_envia_string("Z\n");       
      }//if 'H'
      
      //Grava Tempo Amostra.
      else if (rx == 'J') {
        //Precisa ler 4 caracteres (dígitos) para montar tempo_amostra:
        uint16_t tempo_amostra = 0;
        while (!RCIF); //<<< loop para aguardar o próximo caractere.
        //Caractere (dígito) mais significativo:
        rx = RCREG;
        tempo_amostra = 1000 * (rx - 48); //48 é o código ASCII do caractere zero.
        while (!RCIF); //<<< loop para aguardar o próximo caractere.
        rx = RCREG;
        tempo_amostra = tempo_amostra + 100 * (rx - 48);
        while (!RCIF); //<<< loop para aguardar o próximo caractere.
        rx = RCREG;
        tempo_amostra = tempo_amostra +  10 * (rx - 48);
        while (!RCIF); //<<< loop para aguardar o próximo caractere.
        //Caractere (dígito) menos significativo:
        rx = RCREG;
        tempo_amostra = tempo_amostra +       (rx - 48);
        __delay_ms(50);
        eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, tempo_amostra);
      }//if 'J'
      
  }//if

  if (est_estado_atual == EST_ESTADO_MONITORA ||
      est_estado_atual == EST_ESTADO_MONITORA_GRAVA) {
    
    //Para monitoramento.
    if (rx == 'D') {
      rs232_envia_string("Z\n");
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

