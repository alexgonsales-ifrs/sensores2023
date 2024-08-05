#include "util_menu.h"

/* Inicializa um menu.
 * menu: ponteiro para a variável que representa o menu.
 * itens: ponteiro para o vetor contendo os itens de menu.
 * quant_itens: tamanho do vetor itens. */
void menu_init(TMenu* menu, const TMenuItem* itens, uint8_t quant_itens) {
  menu->index_active    = 0;
  menu->index_nav      = 0;
  menu->quant_itens    = quant_itens;
  menu->itens = (TMenuItem*)(itens);
}//menu_init(())

/* Retorna o indice do item de menu que está sendo mostrado (navegado). */
uint8_t menu_get_index_nav(TMenu* menu) {
  return menu->index_nav;    
}//menu_get_index_nav()

/* Retorna o valor do item de menu ativo (selecionado). */
uint16_t menu_get_value_active(TMenu* menu) {
  return menu->itens[menu->index_active].i_value;
}//menu_get_value_active()

/* Retorna o texto do item de menu que está navegando (mostrado). */
const char* menu_get_text_nav(TMenu* menu) {
  return menu->itens[menu->index_nav].str_text;
}//menu_get_text_nav()

/* Atualiza value, index_active e index_nav para ficar de acordo
 * com o item de menu que tem o valor igual ao valor recebido como parâmetro.  */
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
void menu_restore_index(TMenu* menu) {
  menu->index_nav = menu->index_active;
}//menu_restore_index()

/* Atualiza index_active para o valor de index_nav.
* Usado quando se sai da navegação do menu confirmando com o botão START/ENTER.*/
void menu_confirma_index(TMenu* menu) {
  menu->index_active = menu->index_nav;
}//menu_confirma_index()
