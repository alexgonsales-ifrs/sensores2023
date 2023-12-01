#include <xc.h>  //eeprom_read())
#include <stdio.h> //sprintf()
#include <stdlib.h> //div_t
#include <string.h> //strcat

#include "serv_rs232.h"
#include "rs232.h"
#include "eeprom.h"
#include "adcon.h"
#include "lcd.h"

/*****************************************************************************
 * File:   serv_rs232.c
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

/**
* Envia pela RS232 todas as leituras que estão gravadas na EEPROM e dá mensagem
 * adequada no LCD.
* @return quantidade de bytes transmitidos
*/
uint8_t serv_rs232_envia_leituras_gravadas_eeprom(void) {
  //uint8_t dado;
  uint16_t valor_sensor;
  char str_valor[7] = {0}; //5 caracteres para o valor + 2 para \n = 7.
  char str_linha[25];      //5 caracteres para cada sensor (5x4=20), 3 virgulas, 2 para \n. Total=20+3+2=25.
  char *p_str_valor;
  char *p_str_linha;
  
  lcd_clear();
  lcd_puts("Env. serial...");

  //2400 bauds = (0,000416667 s) * (10 bits em cada transmissao serial) = 4.16 ms
    
  //O java trabalha com inteiros de 16 bits com sinal (little endian) então
  //para facilitar todos os dados serão enviados como words de 16 bits.
    
  //Envia a informação da quantidade de sensores atualmente configurados (Big Endian).
  //rs232_envia_byte(0);
  //rs232_envia_byte(adcon_cfg_quant_sensores_atual);  
  
  uint8_t i;
  p_str_linha = str_linha;
  //Envia pela RS232 as leituras que estão armazenadas na EEPROM.    
  uint8_t num_sensor = 0;
  for (i=0; i < adcon_quant_leituras_gravadas; i = i + 2) {
    num_sensor++;
    //Pega byte mais significativo.
    valor_sensor = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
    valor_sensor = valor_sensor << 8;
    //Pega o byte menos significativo.
    valor_sensor = valor_sensor + eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
    div_t temp_div;
    temp_div =  div((int16_t)valor_sensor, 10);
    
    //Transforma valor para string.
    sprintf(str_valor, "%d.%d", temp_div.quot, temp_div.rem);
    //Concatena na linha e adiciona uma vírgula.
    p_str_valor = str_valor;
    while (*p_str_valor != 0) {
      *p_str_linha = *p_str_valor;
      p_str_linha++;
      p_str_valor++;
    }
    *p_str_linha = ',';
    p_str_linha++;
    *p_str_linha = 0;
    
    strcat(str_linha, str_valor);
    //strcat(str_linha, ",");
    if (num_sensor == adcon_cfg_quant_sensores_atual) {
      //Envia a linha contendo os 4 sensores.
      //strcat(str_linha, "\n");
      *p_str_linha = '\n';
      p_str_linha++;
      *p_str_linha = 0;
      rs232_envia_string(str_linha);
      num_sensor   = 0;
      str_linha[0] = 0; //esvazia a string.
      p_str_linha  = str_linha;
    }
  }//for
  
 /* for (i=0; i < adcon_quant_leituras_gravadas; i = i + 2) {
      //Envia o byte mais significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
      rs232_envia_byte(dado);
      //Envia o byte menos significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
      rs232_envia_byte(dado);
  }//for
*/
  
  lcd_clear();
  sprintf(str_linha, "%d b env.", i);
  lcd_puts(str_linha);

  return i;
    
}//serv_rs232_envia_amostras_gravadas_eeprom()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

