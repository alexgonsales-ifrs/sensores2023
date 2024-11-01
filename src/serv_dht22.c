/*****************************************************************************
 * File:   serv_dht22.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * 2024-10-30
 *
 ****************************************************************************/

//===== Includes =============================================================
#include "serv_dht22.h"
#include <stdio.h>        //sprintf
#include "base_dht22.h"
#include "base_lcd.h"

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

/****************************************************************************
* Faz leitura do DHT22 e mostra no LCD.
 ***************************************************************************/
void serv_dht22_amostra_e_print(void) {
  //if (base_dht22_amostra()) {

  if (base_dht22_amostra())
    serv_dht22_print();

}//serv_dht22_amostra_e_print()

/****************************************************************************
* Mostra no LCD os valores de temperatura e umidade.
 ***************************************************************************/
void serv_dht22_print(void) {
  char temp_str[17] = {0};
  sprintf(temp_str, "t%7d", base_dht22_amostra_temperatura);
  lcd_goto(1, 8);
  lcd_puts(temp_str);

  sprintf(temp_str, "u%7d", base_dht22_amostra_umidade);
  lcd_goto(2, 8);
  lcd_puts(temp_str);

}//serv_dht22_print()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

