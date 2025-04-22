/* 
 * File:   util_menu.h
 * Author: alexdg
 * 
 * Define uma estrutura gen�rica para a implementa��o dos menus da interface do usu�rio.
 *  
 * Um menu (TMenu) cont�m v�rios itens de menu (TMenuItem). Ou seja,
 * um TMenu cont�m um vetor de TMenuItem. Os itens de um menu s�o naveg�veis, ou seja,
 * pode-se fazer uma itera��o para frente ou para tr�s nos itens de menu.
 * 
 * Quando se est� navegando pelos itens de um menu, pode chegar um momento que se quer
 * escolher (ativar) um determinado item de menu. Quando isso acontece dizemos que esse � o item de menu ativado.
 * 
 * Cada item de menu tem uma posi��o (�ndice), que � a posi��o do item dentro do vetor de itens de menu.
 * O campo index_nav da struct TMenu guarda o �ndice do item de menu que est� sendo navegado (mostrado).
 * O campo index_active da struct TMenu guarda o �ndice do item de menu que est� ativado.
 * Quando se inicializa um TMenu, index_nav=0 e index_active=0. 
 * 
 * Quando o usu�rio est� navegando pelos itens de um menu e quer desistir, cancelar essa navega��o,
 * sem modificar o item de menu que estava ativo no momento que ele iniciou a navega��o, � bem prov�vel,
 * que da pr�xima vez que ele retornar a esse menu, vai desejar iniciar a navega��o pelo item de menu ativo
 * e n�o pelo item de menu em que parou a navega��o. Se esse for o caso,
 * ent�o logo ap�s sair da navega��o, devemos chamar a fun��o menu_restore_index() para restaurar index_nav = index_active.
 * Assim, da pr�xima vez que o usu�rio entrar novamente nesse menu, iniciar� a navega��o pelo item de menu ativo.
 * 
 *  
 * menu_init: inicializa um menu. Deve ser a primeira fun��o chamada para se poder utilizar uma estrutura de menus.
 * menu_get_index_nav: retorna o �ndice do item de menu que est� sendo mostrado (navegado). 
 * menu_get_text_nav: retorna o texto do item de menu que est� sendo mostrado (navegado). 
 * menu_get_value_active: retorna o valor do item de menu que est� ativado.
 * menu_inc_index: avan�a para o item de menu seguinte na navega��o.
 * menu_dec_index: retorna para o item de menu anterior na navega��o.
 * menu_confirma_index: ativa o item de menu que est� sendo navegado (mostrado), faz index_active = index_nav.
 * menu_restore_index: faz index_nav = index_active.
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

typedef struct TMenuRaizStruct TMenuRaiz;
typedef struct TMenuStruct     TMenu;
typedef struct TMenuItemStruct TMenuItem;

//Tipos de TMenuItem
//ITEM_MENU_ACAO:   representa um item de menu de a��o (n�o utiliza nenhum campo da union).
//ITEM_MENU_CFG:   representa um item de menu de configura��o, ent�o utiliza o campo i_value da union.
//ITEM_MENU_SUBMENU: representa um submenu, ent�o utiliza o campo p_submenu da union.
typedef enum {MENU_TIPO_ITEM_RAIZ, MENU_TIPO_ITEM_ACAO, MENU_TIPO_ITEM_CFG, MENU_TIPO_ITEM_SUBMENU} TMenuItemTipo;

struct TMenuRaizStruct{
    TMenu      *p_menu_raiz;
    TMenu      *p_menu_atual;   //Menu atual que est� sendo navegado.
    uint8_t     index_nav;      //Para controlar a navega��o nos �tens do menu atual.
    TMenuItem  *p_item_ativado; //Ponteiro para o �ltimo item de menu que foi ativado com a a��o ENTER.
};

  /* TMenu representa um menu com seus itens (TMenuItem) e as vari�veis de controle
   * de navega��o de item de sele��o de item (item ativo).
   */
  struct TMenuStruct{
    uint8_t     index_active; //Indice do item de menu ativo (selecionado).
    //uint8_t     index_nav;    //Indice do item de menu sendo navegado.
    uint8_t     quant_itens;  //Quantidade total de itens de menu (tamanho do vetor itens).
    TMenuItem  *p_itens;        //Ponteiro para o vetor com os itens de menu.
    TMenu      *p_supermenu;  //Ponteiro para o menu pai (supermenu), para saber para onde retornar quando clicar ESC.
    const char *str_titulo;   //Titulo do Menu.
    
    //texto
    //tipo
    //acao
    //
    //i_value
    //p_supermenu
  };
  
  /* TMenuItem representa um item de Menu. Existem diversos tipos de Item de Menu.
   * str_text: texto do item de menu (que ser� mostrado no display).
   * tipo: especifica o tipo de item de menu:
   * tipo==ITEM_MENU_ACAO    ent�o n�o utiliza nenhum campo da union.
   * tipo==ITEM_MENU_CFG     ent�o utiliza o campo i_value da union.
   * tipo==ITEM_MENU_SUBMENU ent�o utiliza o campo p_submenu da union.
   * Descri��o da union:
   * i_value: valor associado ao item de menu, se ele for do tipo ITEM_MENU_CFG.
   * p_submenu: ponteiro para o submenu, caso este item de menu seja do tipo ITEM_MENU_SUBMENU.   
 */
  struct TMenuItemStruct{
    const char    *str_text;
    TMenuItemTipo tipo; 
    uint8_t       acao;       //Se tipo==ITEM_MENU_ACAO ent�o este campo cont�m o c�digo da a��o a ser executada.
    union {
        uint16_t  i_value;    //Se tipo==ITEM_MENU_CFG ent�o este campo cont�m o valor associado ao item de menu.
        TMenu    *p_submenu; //Se tipo==ITEM_MENU_SUBMENU ent�o este campo aponta para esse submenu.
    };
  };
  
//===== Variaveis P�blicas ===================================================

  //extern TMenu *serv_menu_ativo;

//===== Funcoes P�blicas =====================================================
  
extern void menu_raiz_init(TMenuRaiz* p_menu_raiz, TMenu* p_menu);
extern void menu_init(TMenu* p_menu, const TMenuItem* p_itens, uint8_t quant_itens, TMenu *p_supermenu);

extern char*   menu_atual_get_titulo(TMenuRaiz* p_menu_raiz);
extern uint8_t menu_get_tipo_item_menu_ativado(TMenuRaiz* p_menu_raiz);
extern uint8_t menu_get_acao_item_menu_ativado(TMenuRaiz* p_menu_raiz);


//extern uint8_t  menu_get_index_nav(TMenu* p_menu);
extern const char*     menu_get_text_nav(TMenuRaiz* p_menu_raiz);
//extern uint16_t menu_get_value_active(TMenuRaiz* p_menu_raiz);

extern uint16_t menu_get_value_item_ativado(TMenuRaiz* p_menu_raiz);
   
//extern uint16_t menu_get_value_nav(TMenu* p_menu);

//extern void     menu_set_value_indexes(TMenu* p_menu, uint16_t quant);
//extern void     menu_set_index(TMenu* p_menu, uint8_t index);

extern int8_t     menu_exec_down(TMenuRaiz* p_menu_raiz);
extern int8_t     menu_exec_up(TMenuRaiz* p_menu_raiz);
extern void       menu_exec_esc(TMenuRaiz* p_menu_raiz);
extern TMenuItem* menu_exec_enter(TMenuRaiz* p_menu_raiz);

extern void menu_add_submenu(TMenu *p_supermenu, int pos, TMenu *p_submenu);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_MENU_H */
