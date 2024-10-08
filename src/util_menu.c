/*****************************************************************************
 * File:   util_menu.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include "util_menu.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Variaveis P�blicas ===================================================

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

/* Inicializa um menu.
 * menu: ponteiro para a vari�vel que representa o menu.
 * itens: ponteiro para o vetor contendo os itens de menu.
 * quant_itens: tamanho do vetor itens. */
void menu_init(TMenu* menu, const TMenuItem* itens, uint8_t quant_itens) {
  menu->index_active    = 0;
  menu->index_nav      = 0;
  menu->quant_itens    = quant_itens;
  menu->itens = (TMenuItem*)(itens);
}//menu_init(())

/* Retorna o indice do item de menu que est� sendo mostrado (navegado). */
uint8_t menu_get_index_nav(TMenu* menu) {
  return menu->index_nav;    
}//menu_get_index_nav()

/* Retorna o valor do item de menu ativo (selecionado). */
uint16_t menu_get_value_active(TMenu* menu) {
  return menu->itens[menu->index_active].i_value;
}//menu_get_value_active()

/* Retorna o texto do item de menu que est� navegando (mostrado). */
const char* menu_get_text_nav(TMenu* menu) {
  return menu->itens[menu->index_nav].str_text;
}//menu_get_text_nav()

/* Atualiza value, index_active e index_nav para ficar de acordo
 * com o item de menu que tem o valor igual ao valor recebido como par�metro.  */
void menu_set_value_indexes(TMenu* menu, uint16_t value) {
  menu->index_active = 1;  //<<<<<<<<<<<<<<<< remover alexdg 2024-07-25
  for (uint8_t i=0; i< menu->quant_itens; i++) {
    if (menu->itens[i].i_value == value) {
      menu->index_active = i;
      menu->index_nav   = i;
      return;
    }
  }//for
}//menu_set_value_indexes())

/* Incrementa index_nav.
 * Usado durante a navega��o pelo menu. */
int8_t menu_inc_index(TMenu* menu) {
  if (menu->index_nav < menu->quant_itens - 1) {
    menu->index_nav++;
    return (int8_t)(menu->index_nav);
  }
  return -1;
}//menu_inc_index()

/* Decrementa index_nav.
 * Usado durante a navega��o pelo menu. */
int8_t menu_dec_index(TMenu* menu) {
  if (menu->index_nav >= 1) {
    menu->index_nav--;
    return (int8_t)(menu->index_nav);
  }
  return -1;
}//menu_dec_index()

/* Reestabelece o valor de index_nav para index_active.
 * Usado quando se sai da navega��o do menu com o bot�o STOP/ESC. */
void menu_restore_index(TMenu* menu) {
  menu->index_nav = menu->index_active;
}//menu_restore_index()

/* Atualiza index_active para o valor de index_nav.
* Usado quando se sai da navega��o do menu confirmando com o bot�o START/ENTER.*/
void menu_confirma_index(TMenu* menu) {
  menu->index_active = menu->index_nav;
}//menu_confirma_index()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
