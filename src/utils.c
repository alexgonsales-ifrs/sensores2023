/*****************************************************************************
 * File:   utils.c
 * Author: 
 * Comments:
 * 
 * Revision history: 
 *
 ****************************************************************************/

//===== Includes =============================================================
#include "utils.h"

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

void util_uint16_to_str(uint16_t binario, char* p_str_valor) {
  
  p_str_valor[2] = (binario % 10) + 0x30; //0x30 = 48 = c�digo ASCII do caractere zero.
  binario = binario / 10; 
  p_str_valor[1] = (binario % 10) + 0x30; 
  binario = binario / 10;
  p_str_valor[0] = (binario % 10) + 0x30; 
  p_str_valor[3] = 0; //final string
    
}//util_uint16_to_str()

void util_uint8_to_strhex(uint8_t binario, char* p_str_valor) {
  uint8_t b;
  //Pega o nible menos significativo e coloca em p_str_valor[1].
  b = (binario & 0x0F);
  if (b >= 0 && b <= 9) {
    p_str_valor[1] = b + 0x30; //0x30= c�digo ASCII do caractere 0 (zero).
  }
  else { //b>9 E b<15)
    p_str_valor[1] = b + 0x41; //0x41 = c�digo ASCII do caractere A.
  }
  
  //Pega o nible mais significativo e coloca em p_str_valor[0].
  b = (binario & 0xF0) >> 4;
  if (b >= 0 && b <= 9) {
    p_str_valor[0] = b + 0x30; //0x30= c�digo ASCII do caractere 0 (zero).
  }
  else { //b>9 E b<15)
    p_str_valor[0] = b + 0x41; //0x41 = c�digo ASCII do caractere A.
  }
  
  p_str_valor[2] = 0; //final string.
  
}//util_uint8_to_strhex()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

