/*****************************************************************************
 * File:   util_menu.c
 * Author: Alex Dias Gonsales
 * Comments:
 * 
 ****************************************************************************/

//===== Includes =============================================================
#include <stdio.h>  //NULL
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

/* Inicializa um TMenuSystem.
 * p_menu_system : endere�o da vari�vel que representa o sistema de menus.
 * p_menu_raiz: endere�o da vari�vel que cont�m o menu raiz da estrutura de menus. */
void mnu_system_init(TMenuSystem* p_menu_system, TMenu* p_menu_raiz) {
  p_menu_system->p_menu_raiz    = p_menu_raiz;
  p_menu_system->p_menu_atual   = p_menu_raiz;
  p_menu_system->index_nav      = 0;
  p_menu_system->p_item_executado = NULL;
}//mnu_system_init())

/* Retorna o titulo do menu atual.   */
const char* mnu_menu_atual_get_titulo(TMenuSystem* p_menu_system) {
  return p_menu_system->p_menu_atual->str_titulo;
}//mnu_menu_atual_get_titulo()

/* Retorna o tipo do menu atual.  */
TMnuTipoMenu mnu_menu_atual_get_tipo(TMenuSystem* p_menu_system) {
  return p_menu_system->p_menu_atual->tipo;
}//mnu_menu_atual_get_tipo()

/* Atualiza index_active e index_nav do menu atual para ficar de acordo
 * com o item de menu que tem i_value igual ao valor recebido como par�metro. */
void mnu_menu_atual_set_item_ativo_from_value(TMenuSystem* p_menu_system, uint16_t value) {
  TMenu* p_menu;
  p_menu = p_menu_system->p_menu_atual;
  for (uint8_t i=0; i< p_menu->quant_itens; i++) {
    if (p_menu->pv_itens[i].i_value == value) {
      p_menu->index_active    = i;
      p_menu_system->index_nav  = i;
      return;
    }//if
  }//for
}//mnu_menu_atual_set_item_ativo_from_value()

/* Retorna true se o item executado � um submenu.
 * Caso contr�rio retorna false. */
uint8_t mnu_item_executado_is_submenu(TMenuSystem* p_menu_system){
  if (p_menu_system->p_item_executado->p_submenu != NULL)
    return 1;
  else
    return 0;
}//mnu_item_executado_is_submenu()

/* Retorna o c�digo de a��o do item de menu executado. */
uint8_t mnu_item_executado_get_cod_acao(TMenuSystem* p_menu_system){
  TMenuItem*  p_menu_item = p_menu_system->p_item_executado;
  return p_menu_item->cod_acao;
}//mnu_item_executado_get_cod_acao()

/* Retorna o campo i_value do item de menu executado. */
uint16_t mnu_item_executado_get_value(TMenuSystem* p_menu_system) {
  TMenuItem *p_item;
  p_item = p_menu_system->p_item_executado;
  uint16_t i = p_item->i_value;
  return i;
}//mnu_item_executado_get_value()

/* Retorna true se o item navegado � um item de submenu.
 * Caso contr�rio, retorna false. */
uint8_t mnu_item_nav_is_submenu(TMenuSystem* p_menu_system){
  TMenu* p_menu;
  p_menu = (p_menu_system->p_menu_atual);
  uint8_t index;
  index = (p_menu_system->index_nav);
  TMenuItem* p_itens;
  p_itens = &(p_menu->pv_itens[0]);     
  TMenuItem* p_item;
  p_item = (p_itens + index);
  if (p_item->p_submenu != NULL)
    return 1;
  else
    return 0;
}//mnu_item_nav_is_submenu()

/* Retorna o c�digo de a��o do item de menu navegado. */
uint8_t mnu_item_nav_get_cod_acao(TMenuSystem* p_menu_system){
  TMenu* p_menu;
  p_menu = (p_menu_system->p_menu_atual);
  uint8_t index;
  index = (p_menu_system->index_nav);
  TMenuItem* p_itens;
  p_itens = &(p_menu->pv_itens[0]);     
  TMenuItem* p_item;
  p_item = (p_itens + index);
  return p_item->cod_acao;
}//menu_item_nav_get_cod_acao()

/* Retorna o texto do item de menu navegado. */
const char* mnu_item_nav_get_text(TMenuSystem* p_menu_system) {
  TMenu* p_menu;
  p_menu = (p_menu_system->p_menu_atual);
  uint8_t index;
  index = (p_menu_system->index_nav);
  TMenuItem* p_itens;
  p_itens = &(p_menu->pv_itens[0]);     
  //TMenuItem* p_item = &(p_menu->p_itens[index]);
  TMenuItem* p_item;
  p_item = (p_itens + index);
  const char* str_text;
  str_text = (p_item->str_text);
  return str_text;
}//menu_get_text_nav()

/* Navega para o item de menu logo abaixo (incrementa index_nav).
 * Usado durante a navega��o pelo menu. 
 * Retorna o novo �ndice de menu ou -1 se tentou extrapolar o �ndice. */
int8_t mnu_exec_down(TMenuSystem* p_menu_system) {
  TMenu* p_menu = p_menu_system->p_menu_atual;
  uint8_t index = p_menu_system->index_nav;
  if ( index < (p_menu->quant_itens - 1) ) {
    //2025-04-13: Ap�s come�ar a efetuar algumas mudan�as na struct TMenuItem, 
    //come�ou a gerar erro de compila��o ao se usar p�s-incremento (++).
    //error: (712) can't generate code for this expression.
    //menu->index_nav++; //removido e adicionada a linha abaixo.
    index = index + 1;
    p_menu_system->index_nav = index;
    //return (int8_t)(p_menu->index_nav);
    return index;
  }
  return -1;
}//menu_exec_down()

/* Navega para o item de menu logo acima (decrementa index_nav).
 * Usado durante a navega��o pelo menu. 
 * Retorna o novo �ndice de menu ou -1 se tentou extrapolar o �ndice. */
int8_t mnu_exec_up(TMenuSystem* p_menu_system) {
  uint8_t index = p_menu_system->index_nav;
  if (index >= 1) {
    //2025-04-13: Ap�s come�ar a efetuar algumas mudan�as na struct TMenuItem, come�ou a gerar erro de compila��o.
    //error: (712) can't generate code for this expression.
    //menu->index_nav--;
    index = index - 1;
    p_menu_system->index_nav = index;
    //p_menu->index_nav = p_menu->index_nav - 1;
    //return (int8_t)(p_menu->index_nav);
    return index;
  }
  return -1;
}//menu_exec_up()

//<<<<<<<<<<<<<<<< rever esta fun��o caso esteja em um menu de a��o dentro de um submenu (talvez n�o tenha que retornar para o supermenu)
/* Sai de um submenu retornando para o seu menu pai. 
 * Em outras palavras, muda o menu atual para o menu pai do menu atual que estava sendo navegado.
 * Reestabelece o valor de index_nav para o index_active do novo menu atual.*/
void mnu_exec_esc(TMenuSystem* p_menu_system) {
  TMenu *p_menu_atual    = p_menu_system->p_menu_atual;
  //p_menu_raiz->index_nav = p_menu_atual->index_active;  //<<<<< nao precisa
  const TMenu *p_supermenu = p_menu_atual->p_supermenu; //ganha 4 words de programa.
  //Se o item de menu tiver um pai, ent�o retorna para o menu pai.
  if ( p_supermenu != 0) {
    p_menu_system->p_menu_atual = (TMenu*)p_supermenu;
    p_menu_system->index_nav    = p_supermenu->index_active;
    //*pp_menu = p_supermenu;
  }
  //Sen�o, est� no menu principal, permanecer nele.
}//menu_exec_esc()

/*Executa um item de menu, ou seja:
 * - torna esse item o item ativo do menu atual.
 * - torna esse item o item executado do sistema de menus.
 * Se o item executado for um submenu, ent�o entra no submenu, mudando o menu atual.*/
TMenuItem* mnu_exec_enter(TMenuSystem* p_menu_system) {

  //Torna ativo o item de menu que est� sendo navegado (mostrado).
  volatile uint8_t index_active = p_menu_system->index_nav;
  volatile TMenu *p_menu_atual  = p_menu_system->p_menu_atual;
  p_menu_atual->index_active     = index_active;
  
  //Pega o novo item de menu ativo.
  TMenuItem *p_item_menu;
  //p_item_menu1 = &(p_menu_atual->p_itens[index_active]);
  p_item_menu  = &(p_menu_atual->pv_itens[0]);
  p_item_menu  = p_item_menu + index_active;
  //Torna este o item de menu ativado.
  p_menu_system->p_item_executado = p_item_menu;

  //Pega submenu e supermenu para usar nos IFs abaixo.
  const TMenu *p_submenu   = p_item_menu->p_submenu;
  const TMenu *p_supermenu = p_menu_atual->p_supermenu;

  //Realiza a a��o conforme o tipo do item de menu:
  
  //Se o item atual for um submenu ent�o entra no submenu.
  //if ( p_item_menu->tipo == MENU_TIPO_ITEM_SUBMENU && p_submenu != 0) {
  
  //uint8_t tipo;
  //tipo = menu_item_ativado_get_tipo(p_menu_raiz);
  //if ( tipo == MENU_TIPO_ITEM_SUBMENU && p_submenu != 0) {
  
  if ( p_item_menu->p_submenu != NULL) {
    p_menu_system->p_menu_atual = (TMenu*)p_submenu;
    p_menu_system->index_nav    = p_submenu->index_active;
  }
  //Se for um item de configura��o retorna para o menu pai. Testa se tem um menu pai.
  //else if ( p_item_menu->tipo == MENU_TIPO_ITEM_CFG && p_supermenu != 0) {
  //else if ( tipo == MENU_TIPO_ITEM_CFG && p_supermenu != 0) {
  else if ( p_menu_atual->tipo == MNU_TIPO_LISTA && p_supermenu != 0) {
    //Retorna para o menu pai.
    //p_menu_raiz->p_menu_atual = (TMenu*)p_supermenu;
    //p_menu_raiz->index_nav    = p_supermenu->index_active;
  }
  //else if ( p_item_menu->tipo == MENU_TIPO_ITEM_ACAO) {
  else if ( p_menu_atual->tipo == MNU_TIPO_GERAL) {
    //Nada a ser executado.
  }
  return p_item_menu;
}//menu_exec_enter()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
