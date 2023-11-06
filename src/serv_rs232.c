#include <xc.h>  //eeprom_read())
#include <stdio.h> //sprintf()

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
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Vari�veis P�blicas ===================================================

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Vari�veis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

/**
* Envia pela RS232 todas as leituras que est�o gravadas na EEPROM e d� mensagem
 * adequada no LCD.
* @return quantidade de bytes transmitidos
*/
uint8_t serv_rs232_envia_leituras_gravadas_eeprom(void) {
  uint8_t dado;
  char tmp[17] = {0}; //warning do compilador
  
  lcd_clear();
  lcd_puts("Env. serial...");

  //2400 bauds = (0,000416667 s) * (10 bits em cada transmissao serial) = 4.16 ms
    
  //O java trabalha com inteiros de 16 bits com sinal (little endian) ent�o
  //para facilitar todos os dados ser�o enviados como words de 16 bits.
    
  //Envia a informa��o da quantidade de sensores atualmente configurados (Big Endian).
  rs232_envia_byte(0);
  rs232_envia_byte(adcon_cfg_quant_sensores_atual);  
  //Envia pela RS232 as leituras que est�o armazenadas na EEPROM.    
  uint8_t i;
  for (i=0; i < adcon_quant_leituras_gravadas; i = i + 2) {
      //Envia o byte mais significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
      rs232_envia_byte(dado);
      //Envia o byte menos significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
      rs232_envia_byte(dado);
  }//for

  lcd_clear();
  sprintf(tmp, "%d bytes transmitidos", i);
  lcd_puts(tmp);

  return i;
    
}//serv_rs232_envia_amostras_gravadas_eeprom()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

