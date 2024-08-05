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

#include <stdint.h>
    
    //struct TMenu;
    
  /* TMenuItem representa um item de Menu. Um item de menu tem um texto (que será mostrado no display)
   * e um valor associado. Esse valor normalmente é o valor de configuração do item que será
   * utilizado em alguma parte do sistema. */
  typedef struct {
    const char *str_text;
    uint16_t   i_value;
    //struct TMenu * menu;
  } TMenuItem;
  
  /* TMenu representa um menu com seus itens (TMenuItem) e as variáveis de controle
   * de navegação de item de seleção de item (item ativo).
   */
  typedef struct {
    uint8_t     index_active; //Indice do item de menu ativo (selecionado).
    uint8_t     index_nav;    //Indice do item de menu sendo navegado.
    uint8_t     quant_itens;  //Quantidade total de itens de menu (tamanho do vetor itens).
    TMenuItem  *itens;        //Ponteiro para o vetor com os itens de menu.
  } TMenu;
  
extern void menu_init(TMenu* menu, const TMenuItem* itens, uint8_t quant_itens);

extern uint8_t  menu_get_index_nav(TMenu* menu);
const char*     menu_get_text_nav(TMenu* menu);
extern uint16_t menu_get_value_active(TMenu* menu);

extern void     menu_set_value_indexes(TMenu* menu, uint16_t quant);

extern int8_t   menu_inc_index(TMenu* menu);
extern int8_t   menu_dec_index(TMenu* menu);
extern void     menu_restore_index(TMenu* menu);
extern void     menu_confirma_index(TMenu* menu);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_MENU_H */
