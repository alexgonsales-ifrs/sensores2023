/*****************************************************************************
 * File:   ct_prot_rs232.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <xc.h> //
//#include <stdio.h> //sprintf
#include "xtal.h"
#include "string.h"

#include "ct_prot_rs232.h"
#include "ct_estados.h"
#include "base_rs232.h"
#include "base_adcon.h"
#include "serv_adcon.h"

#include "ct_handler.h"
#include "utils.h"


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

/**
 * prot_rs232_executa()
 * Esta função é responsável por ler o comando que foi colocado no buffer da serial,
 * variável hand_vbuffer_rs232, e executar a ação do comando.
 * Após executar a ação o buffer é limpo e a variável hand_qt_buffer_rs232 é zerada.
 */
void prot_rs232_executa(void) {
  char tmp[10];
  //Melhorar esta lógica para funcionar em qualquer ESTADO.
  uint8_t estado_diferente_monitora;
  estado_diferente_monitora =  (est_estado_atual != EST_ESTADO_MONITORA &&
                                est_estado_atual != EST_ESTADO_MONITORA_GRAVA &&
                                est_estado_atual != EST_ESTADO_ENVIAR_DADOS);
  uint8_t rx;
  
  //Le o primeiro caractere do buffer.
  //rx = RCREG;
  //rx = hand_vbuffer_rs232[0];
  rx = 0;
  
  //Copia os 4 primeiros caracteres do buffer e considera-os como o nome do comando.
  char v_nome_comando[5];
  //313 ram
  //6750 rom
  for (int i=0; i<5; i++) {
    v_nome_comando[i] = hand_vbuffer_rs232[i];
  }
  v_nome_comando[4] = 0x00; //final string.
  //311 ram
  //6780 rom
  //strncpy(&v_nome_comando, hand_vbuffer_rs232, 4);

  //Verifica qual o comando recebido:
  
  //Comando AMON
  
  //313 ram
  //7539 rom
  //if (rx) {
  
  //318 ram
  //7558 rom 7558-7539= 19
  //if (v_nome_comando[0]=='A' && v_nome_comando[1]=='M' && v_nome_comando[2]=='O' && v_nome_comando[3]=='N') {
  
  //Comando AMON
  //318 ram
  //7645 rom
  if(strcmp(v_nome_comando, "AMON") == 0) {
  //if(strncmp(v_nome_comando, "AMON", 4) == 0 ) {
    if (estado_diferente_monitora) {
      rs232_envia_string("OKK:AMON\n");
      
      //7558-7521 = 37 words
      //Envia a quantidade de sensores configurados.
      //sprintf(tmp, "QSE=%d\n", adcon_cfg_quant_sensores_atual);
      strcpy(tmp, "QSE=");
      util_uint16_to_str(adcon_cfg_quant_sensores_atual, &tmp[4]);
      strcat(tmp, "\n"); //13 words de ROM
      rs232_envia_string(tmp);
      
      //Envia o tempo entre aquisições.
      //sprintf(tmp, "TAQ=%d\n", adcon_cfg_tempo_aquisicao_atual);
      strcpy(tmp, "TAQ=");
      util_uint16_to_str(adcon_cfg_tempo_aquisicao_atual, &tmp[4]);
      rs232_envia_string(tmp);
      
      //Envia END
      rs232_envia_string("END\n");
      
      //Entra no estado Monitora.
      serv_adcon_bol_monitora_grava = 1;
      est_estado_atual = EST_ESTADO_MONITORA;
      //Habilita monitora.
      INTCONbits.T0IE = 1;
    }//if estado_diferente_monitora
    else {
      //Envia mensagem de erro.
      rs232_envia_string("ERR:AMON monitorando\n");
    }
    
    //limpa o buffer da serial.
    hand_qt_buffer_rs232 = 0;
  }//if 'AMON'

  //Monitora e Grava
  //Comando AMOG
  //318 ram
  //7663 rom
  //else if (v_nome_comando[0]=='A' && v_nome_comando[1]=='M' && v_nome_comando[2]=='O' && v_nome_comando[3]=='N') {
  //318 ram
  //7645 rom 7645-7669= 24
  //else if (rx == 'C') {
  else if(strcmp(v_nome_comando, "AMOG") == 0 ) {
    if (estado_diferente_monitora) {
        rs232_envia_string("Z\n");
        
        //sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        //sprintf(tmp, "T=%d\n", adcon_cfg_tempo_aquisicao_atual);
        rs232_envia_string(tmp);

        //Entra no estado Monitora e Grava.
        est_estado_atual = EST_ESTADO_MONITORA_GRAVA;
        //Habilita monitora.
        INTCONbits.T0IE = 1;
    }//if estado_diferente_monitora
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if 'C'
  
  //Parar monitoramento?
  else if (rx == 'D') {
    if (est_estado_atual == EST_ESTADO_MONITORA ||
       est_estado_atual == EST_ESTADO_MONITORA_GRAVA) {

      rs232_envia_string("Z\n");
      //Para monitoramento.
      est_estado_atual = EST_ESTADO_TRATA_MENUS;
      //Desabilita monitora.
      //INTCONbits.T0IE = 0;
    }//if estado_atual
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if D

      
  //Download? Equipamento envia dados armazenados na EEPROM.
  else if (rx == 'E') {
    if (estado_diferente_monitora) {
        rs232_envia_string("Z\n");
        
        //sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        //sprintf(tmp, "T=%d\n", adcon_cfg_tempo_aquisicao_atual);
        rs232_envia_string(tmp);
        
        //sprintf(tmp, "Q=%d\n", adcon_quant_amostras_gravadas / adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);

        //Envia leituras armazenadas na EEPROM.
        serv_adcon_rs232_envia_amostras_gravadas_eeprom();
    }//if estado_diferente_monitora
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if 'E'

  //LE_QUANT_SENSORES?
  else if (rx == 'F') {
    if (estado_diferente_monitora) {
        rs232_envia_string("Z\n");
        //sprintf(tmp, "S=%d\n", adcon_cfg_quant_sensores_atual);
        rs232_envia_string(tmp);
    }//if estado_diferente_monitora
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if 'F'
      
  //LE_TEMPO_AMOSTRA?
  else if (rx == 'G') {
    if (estado_diferente_monitora) {
        //Envia Tempo Amostra.
        rs232_envia_string("Z\n");
        
        //sprintf(tmp, "T=%d\n", adcon_cfg_tempo_aquisicao_atual);
        rs232_envia_string(tmp);

        //Envia byte mais significativo.
        //rs232_envia_byte((uint8_t)(adcon_cfg_tempo_amostra_atual >> 8));
        //Envia byte menos significativo.
        //rs232_envia_byte((uint8_t)adcon_cfg_tempo_amostra_atual);
    }//if estado_diferente_monitora
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if 'G'
      
  //Grava Quant Sensores.
  else if (rx == 'H') {
    if (estado_diferente_monitora) {
        while (!RCIF); //<<< loop para aguardar o próximo caractere.
        //Le a quant_sensores.
        rx = RCREG;
        rx = rx - 48; //48 é o código ASCII do caractere zero.
        //Grava quant_sensores na EEPROM.
        serv_adcon_set_quant_sensores_atual(rx);
        //adcon_cfg_quant_sensores_atual = rx;
        //eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, rx);
        //Responde
        //sprintf(tmp, "S=%d\n", rx);
        rs232_envia_string("Z\n");
        rs232_envia_string(tmp);
    }//if estado_diferente_monitora
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if 'H'

  //Grava Tempo Amostra.
  else if (rx == 'J') {
    if (estado_diferente_monitora) {
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
        serv_adcon_set_tempo_aquisicao_atual(tempo_amostra);
        //sprintf(tmp, "T=%d\n", tempo_amostra);
        rs232_envia_string("Z\n");        
        rs232_envia_string(tmp);
        //adcon_cfg_tempo_amostra_atual = tempo_amostra;
        //eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, tempo_amostra);
    }//if estado_diferente_monitora
    else {
      //Envia "E=1".
      rs232_envia_string("E=1\n");
    }
  }//if 'J'
  
  //Comando Enviar Dados (comando antigo)?
  else if (rx == 0x41) { //letra 'A'
    serv_adcon_rs232_envia_amostras_gravadas_eeprom();
  }//if 'A'
  
  //Nenhum comando válido
  else {
    //Envia mensagem de erro.
    rs232_envia_string("ERR" "aa");
    //Limpa buffer.
    hand_qt_buffer_rs232 = 0;
  }
  
}//prot_rs232_executa()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

