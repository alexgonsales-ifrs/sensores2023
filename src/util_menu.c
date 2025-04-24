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

//TMenu *serv_menu_ativo;

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

void menu_raiz_init(TMenuRaiz* p_menu_raiz, TMenu* p_menu) {
  p_menu_raiz->p_menu_raiz    = p_menu;
  p_menu_raiz->p_menu_atual   = p_menu;
  p_menu_raiz->index_nav      = 0;
  p_menu_raiz->p_item_ativado = 0;
}

/* Inicializa um menu.
 * p_menu: ponteiro para a variável que representa o menu.
 * p_itens: ponteiro para o vetor contendo os itens de menu.
 * quant_itens: tamanho do vetor itens.
 * p_supermenu: ponteiro para o menu pai, caso este item seja um submenu.
 *  */
void menu_init(TMenu* p_menu, const TMenuItem* p_itens, uint8_t quant_itens, TMenu *p_supermenu) {
  p_menu->index_active = 0;
  //p_menu->index_nav  = 0;
  p_menu->quant_itens  = quant_itens;
  p_menu->p_itens      = p_itens;
  p_menu->p_supermenu  = p_supermenu;
}//menu_init(())


/* Retorna o valor do item de menu ativo (selecionado). */
//uint16_t menu_get_value_active(TMenu* p_menu) {
//  return p_menu->p_itens[p_menu->index_active].i_value;
//}//menu_get_value_active()

/* Retorna o valor do item de menu ativo (selecionado). */
/*uint16_t menu_get_value_active(TMenuRaiz* p_menu_raiz) {
  TMenu *p_menu;
  p_menu = p_menu_raiz->p_menu_atual;
  uint8_t index = p_menu->index_active;
  uint16_t i = p_menu->p_itens[index].i_value;
  return i;
}//menu_get_value_active()
*/

char* menu_atual_get_titulo(TMenuRaiz* p_menu_raiz) {
  return p_menu_raiz->p_menu_atual->str_titulo;
}

uint8_t menu_get_tipo_item_menu_ativado(TMenuRaiz* p_menu_raiz){
  TMenuItem*  p_menu_item = p_menu_raiz->p_item_ativado;
  return p_menu_item->tipo;
}

uint8_t menu_get_acao_item_menu_ativado(TMenuRaiz* p_menu_raiz){
  TMenuItem*  p_menu_item = p_menu_raiz->p_item_ativado;
  return p_menu_item->acao;
}

/* Retorna o valor do último item de menu que foi ativado. */
uint16_t menu_get_value_item_ativado(TMenuRaiz* p_menu_raiz) {
  TMenuItem *p_item;
  p_item = p_menu_raiz->p_item_ativado;
  uint16_t i = p_item->i_value;
  return i;
}//menu_get_value_item_ativado()

/* Retorna o indice do item de menu que está sendo mostrado (navegado). */
//uint8_t menu_get_index_nav(TMenu* p_menu) {
//  return p_menu->index_nav;    
//}//menu_get_index_nav()

/* Retorna o valor do item de menu mostrado (navegado). */
//uint16_t menu_get_value_nav(TMenu* p_menu) {
//  return p_menu->p_itens[p_menu->index_nav].i_value;
//}//menu_get_value_nav()

/* Retorna o texto do item de menu que está navegando (mostrado). */
const char* menu_get_text_nav(TMenuRaiz* p_menu_raiz) {
  TMenu* p_menu;
  p_menu = (p_menu_raiz->p_menu_atual);
  uint8_t index;
  index = (p_menu_raiz->index_nav);
  TMenuItem* p_itens;
  p_itens = (p_menu->p_itens);     //aqui está dando "invalid address".
  //TMenuItem* p_item = &(p_menu->p_itens[index]);
  TMenuItem* p_item;
  p_item = (p_itens + index);
  const char* str_text;
  str_text = (p_item->str_text);
  return str_text;
}//menu_get_text_nav()

/*
void menu_set_index(TMenu* menu, uint8_t index) {
  if (index >=0 ) {
    menu->index_active = index;
    menu->index_nav    = index;
  }
}//menu_set_index())
*/

/* Atualiza value, index_active e index_nav para ficar de acordo
 * com o item de menu que tem o valor igual ao valor recebido como parâmetro.  */
/*void menu_set_value_indexes(TMenu* p_menu, uint16_t value) {
//  menu->index_active = 1;  //<<<<<<<<<<<<<<<< remover alexdg 2024-07-25
  for (uint8_t i=0; i< p_menu->quant_itens; i++) {
    if (p_menu->p_itens[i].i_value == value) {
      p_menu->index_active = i;
      p_menu->index_nav   = i;
      return;
    }
  }//for
}//menu_set_value_indexes())
*/

/* Incrementa index_nav.
 * Usado durante a navegação pelo menu. 
 * Retorna o novo índice de menu ou -1 se tentou extrapolar o índice.
 */
int8_t menu_exec_down(TMenuRaiz* p_menu_raiz) {
  TMenu* p_menu = p_menu_raiz->p_menu_atual;
  uint8_t index = p_menu_raiz->index_nav;
  if ( index < (p_menu->quant_itens - 1) ) {
    //2025-04-13: Após começar a efetuar algumas mudanças na struct TMenuItem, 
    //começou a gerar erro de compilação ao se usar pós-incremento (++).
    //error: (712) can't generate code for this expression.
    //menu->index_nav++; //removido e adicionada a linha abaixo.
    index = index + 1;
    p_menu_raiz->index_nav = index;
    //return (int8_t)(p_menu->index_nav);
    return index;
  }
  return -1;
}//menu_exec_down()

/* Decrementa index_nav.
 * Usado durante a navegação pelo menu. 
 * Retorna o novo índice de menu ou -1 se tentou extrapolar o índice.
 */
int8_t menu_exec_up(TMenuRaiz* p_menu_raiz) {
  uint8_t index = p_menu_raiz->index_nav;
  if (index >= 1) {
    //2025-04-13: Após começar a efetuar algumas mudanças na struct TMenuItem, começou a gerar erro de compilação.
    //error: (712) can't generate code for this expression.
    //menu->index_nav--;
    index = index - 1;
    p_menu_raiz->index_nav = index;
    //p_menu->index_nav = p_menu->index_nav - 1;
    //return (int8_t)(p_menu->index_nav);
    return index;
  }
  return -1;
}//menu_exec_up()

/* Reestabelece o valor de index_nav para index_active.
 * Usado quando se sai da navegação do menu com o botão STOP/ESC. */
/*
void menu_restore_index(TMenu* menu) {
  menu->index_nav = menu->index_active;
}//menu_restore_index()
*/


//<<<<<<<<<<<<<<<< rever esta função caso esteja em um menu de ação dentro de um submenu (talvez não tenha que retornar para o supermenu)
/* Reestabelece o valor de index_nav para index_active.
 * Usado quando se sai da navegação do menu com o botão STOP/ESC. */
void menu_exec_esc(TMenuRaiz* p_menu_raiz) {
  TMenu *p_menu_atual    = p_menu_raiz->p_menu_atual;
  //p_menu_raiz->index_nav = p_menu_atual->index_active;  //<<<<< nao precisa
  TMenu *p_supermenu = p_menu_atual->p_supermenu; //ganha 4 words de programa.
  //Se o item de menu tiver um pai, então retorna para o menu pai.
  if ( p_supermenu != 0) {
    p_menu_raiz->p_menu_atual = p_supermenu;
    p_menu_raiz->index_nav    = p_supermenu->index_active;
    //*pp_menu = p_supermenu;
  }
  //Senão, está no menu principal, permanecer nele.
  
}//menu_exec_esc()

/* Atualiza index_active para o valor de index_nav.
* Usado quando se sai da navegação do menu confirmando com o botão START/ENTER.*/
/*
void menu_confirma_index(TMenu* menu) {
  menu->index_active = menu->index_nav;
}//menu_confirma_index()
*/

/* Atualiza index_active para o valor de index_nav.
* Usado quando se sai da navegação do menu confirmando com o botão START/ENTER.*/
TMenuItem* menu_exec_enter(TMenuRaiz* p_menu_raiz) {

  //Torna ativo o item de menu que está sendo navegado (mostrado).
  volatile uint8_t index_active = p_menu_raiz->index_nav;
  volatile TMenu *p_menu_atual  = p_menu_raiz->p_menu_atual;
  p_menu_atual->index_active     = index_active;
  
  //Pega o novo item de menu ativo.
  TMenuItem *p_item_menu1;
  TMenuItem *p_item_menu;
  //p_item_menu1 = &(p_menu_atual->p_itens[index_active]);
  p_item_menu  = &(p_menu_atual->p_itens[0]);
  p_item_menu  = p_item_menu + index_active;
  //Torna este o item de menu ativado.
  p_menu_raiz->p_item_ativado = p_item_menu;

  //Pega submenu e supermenu para usar nos IFs abaixo.
  volatile TMenu *p_submenu   = p_item_menu->p_submenu;
  volatile TMenu *p_supermenu = p_menu_atual->p_supermenu;

  //Realiza a ação conforme o tipo do item de menu:
  
  //Se for um submenu, entra no submenu.
  if ( p_item_menu->tipo == MENU_TIPO_ITEM_SUBMENU && p_submenu != 0) {
    p_menu_raiz->p_menu_atual = p_submenu;
    p_menu_raiz->index_nav    = p_submenu->index_active;
  }
  //Se for um item de configuração retorna para o menu pai. Testa se tem um menu pai.
  else if ( p_item_menu->tipo == MENU_TIPO_ITEM_CFG && p_supermenu != 0) {
    //Retorna para o menu pai.
    p_menu_raiz->p_menu_atual = p_supermenu;
    p_menu_raiz->index_nav    = p_supermenu->index_active;
  }
  else if ( p_item_menu->tipo == MENU_TIPO_ITEM_ACAO) {
    //Nada a ser executado.
  }
  return p_item_menu;
}//menu_exec_enter()

/* Especifica que o item "pos" do menu_pai tem um p_submenu.
 */
void menu_add_submenu(TMenu *p_supermenu, int pos, TMenu *p_submenu) {
  //p_supermenu->p_itens[pos].p_submenu = p_submenu;
  //p_submenu->p_supermenu = p_supermenu;  
  
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
