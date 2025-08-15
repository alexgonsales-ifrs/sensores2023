/* File:   util_menu.h
 * Author: Alex Dias Gonsales
 * 
 * Implementa uma biblioteca de fun��es para se criar um sistema de menus.
 *  
 * Um menu (TMenu) cont�m v�rios itens de menu (TMenuItem), ou seja,
 * um TMenu cont�m um vetor de TMenuItem. Os itens de um menu s�o naveg�veis, ou seja,
 * pode-se fazer uma intera��o para frente ou para tr�s nos itens de menu.
 * 
 */

#ifndef UTIL_MENU_H
#define	UTIL_MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
    
#include <stdint.h>

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================

//===== Tipos P�blicos =======================================================

//Tipos de Menus/Submenus:
typedef enum {
  MNU_TIPO_GERAL, //Menu pode conter itens de submenu e itens de funcionalidade (embora n�o deveria conter itens de configura��o).
  MNU_TIPO_LISTA, //Menu deve conter apenas itens de configura��o.
  MNU_TIPO_FAIXA, //N�o utilizado ainda.
} TMnuTipoMenu;

//Declara��o antecipada para n�o dar erro nas defini��es das structs abaixo.
typedef struct TMenuSystem TMenuSystem;
typedef struct TMenu       TMenu;
typedef struct TMenuItem   TMenuItem;

/* TMenuSistemStruct representa o ponto de entrada do sistema de menus implementado pela biblioteca.
 */
struct TMenuSystem {
  TMenu      *p_menu_raiz;      //Menu ra�z (ponto de entrada) da estrutura de menus.
  TMenu      *p_menu_atual;     //Menu atual que est� sendo navegado.
  TMenuItem  *p_item_executado; //Ponteiro para o item de menu mais recente que foi executado.
  uint8_t     index_nav;        //�ndice do item de menu atual que est� sendo navegado no p_menu_atual.
};

/* TMenu representa um menu com seus itens (TMenuItem) e as vari�veis de controle
 * para navega��o pelos itens.
 */
struct TMenu {
  const TMnuTipoMenu  tipo;         //Especifica o tipo de Menu (vide TMnuTipoMenu).
  uint8_t             index_active; //Indice do item de menu ativo.
  uint8_t             quant_itens;  //Quantidade total de itens de menu (tamanho do vetor pv_itens).
  const TMenuItem    *pv_itens;     //Ponteiro para o primeiro elemento do vetor contendo os itens de menu.
  const TMenu        *p_supermenu;  //Ponteiro para o menu pai (supermenu).
  const char         *str_titulo;   //Titulo do Menu.
};

/* TMenuItem representa um item de Menu. 
 */
struct TMenuItem {
  const char*         str_text;  //Texto do item de menu.
  const uint8_t       cod_acao;  //C�digo que representa a a��o que dever� ser executada quando o item de menu for executado.
  const uint16_t      i_value;   //Valor auxiliar para a a��o a ser executada.
  const TMenu*        p_submenu; //Se o item for um submenu ent�o este campo aponta para o submenu.
};

//===== Variaveis P�blicas ===================================================

//===== Funcoes P�blicas =====================================================
  
extern void mnu_system_init(TMenuSystem* p_menu_system, TMenu* p_menu_raiz);

extern const char*   mnu_menu_atual_get_titulo(TMenuSystem* p_menu_system);
extern void          mnu_menu_atual_set_item_ativo_from_value(TMenuSystem* p_menu_system, uint16_t value);
extern TMnuTipoMenu  mnu_menu_atual_get_tipo(TMenuSystem* p_menu_system);

extern uint8_t       mnu_item_executado_get_cod_acao(TMenuSystem* p_menu_system);
extern uint16_t      mnu_item_executado_get_value(TMenuSystem* p_menu_system);
extern uint8_t       mnu_item_executado_is_submenu(TMenuSystem* p_menu_system);

extern uint8_t       mnu_item_nav_get_cod_acao(TMenuSystem* p_menu_system);
extern uint8_t       mnu_item_nav_is_submenu(TMenuSystem* p_menu_system);
extern const char*   mnu_item_nav_get_text(TMenuSystem* p_menu_system);

extern int8_t     mnu_exec_down(TMenuSystem* p_menu_system);
extern int8_t     mnu_exec_up(TMenuSystem* p_menu_system);
extern void       mnu_exec_esc(TMenuSystem* p_menu_system);
extern TMenuItem* mnu_exec_enter(TMenuSystem* p_menu_system);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_MENU_H */
