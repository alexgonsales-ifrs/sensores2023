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

//#include "ct_handler.h"
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
 * Esta função é responsável interpretar e executar o comando do protocolo
 * da rs232.
 * @param p_nome_comando
 */
void prot_rs232_executa(const char *p_str_comando) {
  char tmp[10];
  //Melhorar esta lógica para funcionar em qualquer ESTADO.
  uint8_t estado_diferente_monitora;
  estado_diferente_monitora =  (est_estado_atual != EST_ESTADO_MONITORA &&
                                est_estado_atual != EST_ESTADO_MONITORA_GRAVA &&
                                est_estado_atual != EST_ESTADO_ENVIAR_DADOS);

  //Remover rx depois que não for mais necessário.
  uint8_t rx;
  rx = 0;
  
  //================ Comando AMON =============================
  //Compara os 4 primeiros caracters do comando:
  if(strncmp(p_str_comando, "AMON", 4) == 0) {
    if (estado_diferente_monitora) {
      rs232_envia_string("OKK:AMON\n");
      //Monta a string QSE=x\n
      //onde x é a quantidade de sensores.
      strcpy(tmp, "QSE=");
      //Coloca o valor a partir da posição 4 do vetor tmp[].
      util_uint16_to_str(adcon_cfg_quant_sensores_atual, &tmp[4]);
      //Adiciona um final de linha \n.
      strcat(tmp, "\n"); //13 words de ROM
      //Envia a string pela serial.
      rs232_envia_string(tmp);
      
      //Envia o tempo entre aquisições.
      //sprintf(tmp, "TAQ=%d\n", adcon_cfg_tempo_aquisicao_atual);
      strcpy(tmp, "TAQ=");
      util_uint16_to_str(adcon_cfg_tempo_aquisicao_atual, &tmp[4]);
      strcat(tmp, "\n");
      rs232_envia_string(tmp);
      
      //Envia END
      rs232_envia_string("END\n");
      
      //Entra no estado Monitora.
      serv_adcon_bol_monitora_grava = 1;
      est_estado_atual = EST_ESTADO_MONITORA;
      INTCONbits.T0IE = 1; //Habilita monitora.
    }//if estado_diferente_monitora
    else {
      //Envia mensagem de erro.
      rs232_envia_string("ERR:AMON monitorando\n");
    }
    
    
  }//if 'AMON'

  //Monitora e Grava
  //Comando AMOG
  //318 ram
  //7663 rom
  //else if (v_nome_comando[0]=='A' && v_nome_comando[1]=='M' && v_nome_comando[2]=='O' && v_nome_comando[3]=='N') {
  //318 ram
  //7645 rom 7645-7669= 24
  //else if (rx == 'C') {
  else if(strcmp(p_str_comando, "AMOG") == 0 ) {
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
  }
  
}//prot_rs232_executa()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

