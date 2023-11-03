# 1 "src/menu.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/opt/microchip/mplabx/v6.15/packs/Microchip/PIC16Fxxx_DFP/1.4.149/xc8/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "src/menu.c" 2
# 1 "src/menu.h" 1
# 16 "src/menu.h"
# 1 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 1 3
# 13 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef signed char int8_t;






typedef signed int int16_t;







typedef __int24 int24_t;







typedef signed long int int32_t;
# 52 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef unsigned char uint8_t;





typedef unsigned int uint16_t;






typedef __uint24 uint24_t;






typedef unsigned long int uint32_t;
# 88 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef signed char int_least8_t;







typedef signed int int_least16_t;
# 109 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef __int24 int_least24_t;
# 118 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef signed long int int_least32_t;
# 136 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef unsigned char uint_least8_t;






typedef unsigned int uint_least16_t;
# 154 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef __uint24 uint_least24_t;







typedef unsigned long int uint_least32_t;
# 181 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef signed char int_fast8_t;






typedef signed int int_fast16_t;
# 200 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef __int24 int_fast24_t;







typedef signed long int int_fast32_t;
# 224 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef unsigned char uint_fast8_t;





typedef unsigned int uint_fast16_t;
# 240 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef __uint24 uint_fast24_t;






typedef unsigned long int uint_fast32_t;
# 268 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef int32_t intmax_t;
# 282 "/opt/microchip/xc8/v2.41/pic/include/c90/stdint.h" 3
typedef uint32_t uintmax_t;






typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 17 "src/menu.h" 2




  typedef struct {
    const char *str_text;
    uint16_t i_value;
  } TMenuItem;




  typedef struct {
    uint8_t index_active;
    uint8_t index_nav;
    uint8_t quant_itens;
    TMenuItem *itens;
  } TMenu;

extern void menu_init(TMenu* menu, const TMenuItem* itens, uint8_t quant_itens);

extern uint16_t menu_get_value_active(TMenu* menu);
extern uint8_t menu_get_index_nav(TMenu* menu);
extern void menu_set_value_indexes(TMenu* menu, uint16_t quant);

extern int8_t menu_inc_index(TMenu* menu);
extern int8_t menu_dec_index(TMenu* menu);
extern void menu_restore_index(TMenu* menu);
extern void menu_confirma_index(TMenu* menu);
# 2 "src/menu.c" 2





void menu_init(TMenu* menu, const TMenuItem* itens, uint8_t quant_itens) {
  menu->index_active = 0;
  menu->index_nav = 0;
  menu->quant_itens = quant_itens;
  menu->itens = itens;
}


uint8_t menu_get_index_nav(TMenu* menu) {
  return menu->index_nav;
}


uint16_t menu_get_value_active(TMenu* menu) {
  return menu->itens[menu->index_active].i_value;
}



void menu_set_value_indexes(TMenu* menu, uint16_t value) {
  menu->index_active = 1;
  for (uint8_t i=0; i< menu->quant_itens; i++) {
    if (menu->itens[i].i_value == value) {
      menu->index_active = i;
      menu->index_nav = i;
      return;
    }
  }
}



int8_t menu_inc_index(TMenu* menu) {
  if (menu->index_nav < menu->quant_itens - 1) {
    menu->index_nav++;
    return (int8_t)(menu->index_nav);
  }
  return -1;
}



int8_t menu_dec_index(TMenu* menu) {
  if (menu->index_nav >= 1) {
    menu->index_nav--;
    return (int8_t)(menu->index_nav);
  }
  return -1;
}



void menu_restore_index(TMenu* menu) {
  menu->index_nav = menu->index_active;
}



void menu_confirma_index(TMenu* menu) {
  menu->index_active = menu->index_nav;
}
