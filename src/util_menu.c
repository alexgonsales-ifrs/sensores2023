/*****************************************************************************
 * File:   util_menu.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================
#include <stdio.h>  //NULL
#include "util_menu.h"

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variaveis Públicas ===================================================

TMenu *menu_p_menu_ativo;

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

/* Inicializa um menu.
 * p_menu: ponteiro para a variável que representa o menu.
 * p_itens: ponteiro para o vetor contendo os itens de menu.
 * quant_itens: tamanho do vetor itens.
 * p_supermenu: ponteiro para o menu pai, caso este item seja um submenu.
 *  */
void menu_init(TMenu* p_menu, const TMenuItem* p_itens, uint8_t quant_itens, TMenu *p_supermenu) {
  p_menu->index_active   = 0;
  p_menu->index_nav      = 0;
  p_menu->quant_itens    = quant_itens;
  p_menu->itens = (TMenuItem*)(p_itens);
  p_menu->p_supermenu = p_supermenu;
}//menu_init(())

/* Retorna o indice do item de menu que está sendo mostrado (navegado). */
uint8_t menu_get_index_nav(TMenu* menu) {
  return menu->index_nav;    
}//menu_get_index_nav()

/* Retorna o valor do item de menu ativo (selecionado). */
uint16_t menu_get_value_active(TMenu* menu) {
  return menu->itens[menu->index_active].i_value;
}//menu_get_value_active()

/* Retorna o valor do item de menu mostrado (navegado). */
uint16_t menu_get_value_nav(TMenu* menu) {
  return menu->itens[menu->index_nav].i_value;
}//menu_get_value_nav()

/* Retorna o texto do item de menu que está navegando (mostrado). */
const char* menu_get_text_nav(TMenu* menu) {
  return menu->itens[menu->index_nav].str_text;
}//menu_get_text_nav()

void menu_set_index(TMenu* menu, uint8_t index) {
  if (index >=0 ) {
    menu->index_active = index;
    menu->index_nav    = index;
  }
}//menu_set_index())

/* Atualiza value, index_active e index_nav para ficar de acordo
 * com o item de menu que tem o valor igual ao valor recebido como parâmetro.  */
void menu_set_value_indexes(TMenu* menu, uint16_t value) {
//  menu->index_active = 1;  //<<<<<<<<<<<<<<<< remover alexdg 2024-07-25
  for (uint8_t i=0; i< menu->quant_itens; i++) {
    if (menu->itens[i].i_value == value) {
      menu->index_active = i;
      menu->index_nav   = i;
      return;
    }
  }//for
}//menu_set_value_indexes())

/* Incrementa index_nav.
 * Usado durante a navegação pelo menu. */
int8_t menu_inc_index(TMenu* menu) {
  if (menu->index_nav < menu->quant_itens - 1) {
    menu->index_nav++;
    return (int8_t)(menu->index_nav);
  }
  return -1;
}//menu_inc_index()

/* Decrementa index_nav.
 * Usado durante a navegação pelo menu. */
int8_t menu_dec_index(TMenu* menu) {
  if (menu->index_nav >= 1) {
    menu->index_nav--;
    return (int8_t)(menu->index_nav);
  }
  return -1;
}//menu_dec_index()

/* Reestabelece o valor de index_nav para index_active.
 * Usado quando se sai da navegação do menu com o botão STOP/ESC. */
/*
void menu_restore_index(TMenu* menu) {
  menu->index_nav = menu->index_active;
}//menu_restore_index()
*/

/* Reestabelece o valor de index_nav para index_active.
 * Usado quando se sai da navega??o do menu com o bot?o STOP/ESC. */
void menu_restore_index(TMenu* menu) {
  menu->index_nav = menu->index_active;
  //Se o item de menu tiver um pai, ent?o retorna para o menu pai.
  if ( (menu->p_supermenu) != 0) {
    menu_p_menu_ativo = menu->p_supermenu;
  }
  //Sen?o, est? no menu principal, permanecer nele.
  
}//menu_restore_index()

/* Atualiza index_active para o valor de index_nav.
* Usado quando se sai da navegação do menu confirmando com o botão START/ENTER.*/
/*
void menu_confirma_index(TMenu* menu) {
  menu->index_active = menu->index_nav;
}//menu_confirma_index()
*/

/* Atualiza index_active para o valor de index_nav.
* Usado quando se sai da navega??o do menu confirmando com o bot?o START/ENTER.*/
void menu_confirma_index(TMenu* menu) {
  //Torna ativo o item de menu que está sendo navegado (mostrado).
  menu->index_active = menu->index_nav;
  //Se o item de menu for um submenu, entra no submenu.
  if ( (menu->itens[menu->index_active].p_submenu) != 0) {
    menu_p_menu_ativo = menu->itens[menu->index_active].p_submenu;
  }
  //Se o item de menu não for um submenu e o campo retorno_pai == 1 então 
  //retorna para o menu pai.
  else if (menu->itens[menu->index_active].retorno_pai == 1) {
    //Se o item de menu tiver um menu pai, retorna para o menu pai.
    if (menu->p_supermenu != 0) {
      //Retorna para o menu pai.
      menu_p_menu_ativo = menu_p_menu_ativo->p_supermenu;
    }
  }
}//menu_confirma_index()

/* Especifica que o item "pos" do menu_pai tem um p_submenu.
 */
void menu_add_submenu(TMenu *p_supermenu, int pos, TMenu *p_submenu) {
  p_supermenu->itens[pos].p_submenu = p_submenu;
  p_submenu->p_supermenu = p_supermenu;  
  //TMenuItem *i;
  //i = p_supermenu->itens;
  //i++;
  //i++;
  //i++;
  //i++;
  //i++;
  //i->i_value = 5;
  //i->p_submenu = 0;
}
//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
