/* 
 * File:   util_menu.h
 * Author: alexdg
 * 
 * Define uma estrutura genérica para a implementação dos menus da interface do usuário.
 *  
 * Um menu (TMenu) contém vários itens de menu (TMenuItem). Ou seja,
 * um TMenu contém um vetor de TMenuItem. Os itens de um menu são navegáveis, ou seja,
 * pode-se fazer uma iteração para frente ou para trás nos itens de menu.
 * 
 * Quando se está navegando pelos itens de um menu, pode chegar um momento que se quer
 * escolher (ativar) um determinado item de menu. Quando isso acontece dizemos que esse é o item de menu ativado.
 * 
 * Cada item de menu tem uma posição (índice), que é a posição do item dentro do vetor de itens de menu.
 * O campo index_nav da struct TMenu guarda o índice do item de menu que está sendo navegado (mostrado).
 * O campo index_active da struct TMenu guarda o índice do item de menu que está ativado.
 * Quando se inicializa um TMenu, index_nav=0 e index_active=0. 
 * 
 * Quando o usuário está navegando pelos itens de um menu e quer desistir, cancelar essa navegação,
 * sem modificar o item de menu que estava ativo no momento que ele iniciou a navegação, é bem provável,
 * que da próxima vez que ele retornar a esse menu, vai desejar iniciar a navegação pelo item de menu ativo
 * e não pelo item de menu em que parou a navegação. Se esse for o caso,
 * então logo após sair da navegação, devemos chamar a função menu_restore_index() para restaurar index_nav = index_active.
 * Assim, da próxima vez que o usuário entrar novamente nesse menu, iniciará a navegação pelo item de menu ativo.
 * 
 *  
 * menu_init: inicializa um menu. Deve ser a primeira função chamada para se poder utilizar uma estrutura de menus.
 * menu_get_index_nav: retorna o índice do item de menu que está sendo mostrado (navegado). 
 * menu_get_text_nav: retorna o texto do item de menu que está sendo mostrado (navegado). 
 * menu_get_value_active: retorna o valor do item de menu que está ativado.
 * menu_inc_index: avança para o item de menu seguinte na navegação.
 * menu_dec_index: retorna para o item de menu anterior na navegação.
 * menu_confirma_index: ativa o item de menu que está sendo navegado (mostrado), faz index_active = index_nav.
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
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Constantes Públicas ==================================================

//===== Tipos Públicos =======================================================

typedef struct TMenuRaizStruct TMenuRaiz;
typedef struct TMenuStruct     TMenu;
typedef struct TMenuItemStruct TMenuItem;

//Tipos de TMenuItem
//ITEM_MENU_ACAO:   representa um item de menu de ação (não utiliza nenhum campo da union).
//ITEM_MENU_CFG:   representa um item de menu de configuração, então utiliza o campo i_value da union.
//ITEM_MENU_SUBMENU: representa um submenu, então utiliza o campo p_submenu da union.
typedef enum {MENU_TIPO_ITEM_RAIZ, MENU_TIPO_ITEM_ACAO, MENU_TIPO_ITEM_CFG, MENU_TIPO_ITEM_SUBMENU} TMenuItemTipo;

struct TMenuRaizStruct{
    TMenu      *p_menu_raiz;
    TMenu      *p_menu_atual;   //Menu atual que está sendo navegado.
    uint8_t     index_nav;      //Para controlar a navegação nos ítens do menu atual.
    TMenuItem  *p_item_ativado; //Ponteiro para o último item de menu que foi ativado com a ação ENTER.
};

  /* TMenu representa um menu com seus itens (TMenuItem) e as variáveis de controle
   * de navegação de item de seleção de item (item ativo).
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
   * str_text: texto do item de menu (que será mostrado no display).
   * tipo: especifica o tipo de item de menu:
   * tipo==ITEM_MENU_ACAO    então não utiliza nenhum campo da union.
   * tipo==ITEM_MENU_CFG     então utiliza o campo i_value da union.
   * tipo==ITEM_MENU_SUBMENU então utiliza o campo p_submenu da union.
   * Descrição da union:
   * i_value: valor associado ao item de menu, se ele for do tipo ITEM_MENU_CFG.
   * p_submenu: ponteiro para o submenu, caso este item de menu seja do tipo ITEM_MENU_SUBMENU.   
 */
  struct TMenuItemStruct{
    const char    *str_text;
    TMenuItemTipo tipo; 
    uint8_t       acao;       //Se tipo==ITEM_MENU_ACAO então este campo contém o código da ação a ser executada.
    union {
        uint16_t  i_value;    //Se tipo==ITEM_MENU_CFG então este campo contém o valor associado ao item de menu.
        TMenu    *p_submenu; //Se tipo==ITEM_MENU_SUBMENU então este campo aponta para esse submenu.
    };
  };
  
//===== Variaveis Públicas ===================================================

  //extern TMenu *serv_menu_ativo;

//===== Funcoes Públicas =====================================================
  
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
