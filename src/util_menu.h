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

#include <stdint.h>
    
    //struct TMenu;
    
  /* TMenuItem representa um item de Menu. Um item de menu tem um texto (que ser� mostrado no display)
   * e um valor associado. Esse valor normalmente � o valor de configura��o do item que ser�
   * utilizado em alguma parte do sistema. */
  typedef struct {
    const char *str_text;
    uint16_t   i_value;
    //struct TMenu * menu;
  } TMenuItem;
  
  /* TMenu representa um menu com seus itens (TMenuItem) e as vari�veis de controle
   * de navega��o de item de sele��o de item (item ativo).
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
