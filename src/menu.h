/* 
 * File:   menu.h
 * Author: alexdg
 * 
 * Define uma estrutura genérica para a implementação dos menus da interface do usuário.
 *  
 */

#ifndef MENU_H
#define	MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
  /* TMenuItem representa um item de Menu. Um item de menu tem um texto (que será mostrado no display)
   * e um valor associado. Esse valor normalmente é o valor de configuração do item que será
   * utilizado em alguma parte do sistema. */
  typedef struct {
    const char *str_text;
    uint16_t   i_value;
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

extern uint16_t menu_get_value_active(TMenu* menu);
extern uint8_t  menu_get_index_nav(TMenu* menu);
extern void     menu_set_value_indexes(TMenu* menu, uint16_t quant);

extern int8_t   menu_inc_index(TMenu* menu);
extern int8_t   menu_dec_index(TMenu* menu);
extern void     menu_restore_index(TMenu* menu);
extern void     menu_confirma_index(TMenu* menu);

#ifdef	__cplusplus
}
#endif

#endif	/* MENU_QUANT_SENSORES_H */
