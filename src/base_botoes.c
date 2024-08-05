#include <xc.h>
#include "base_botoes.h"
#include "versao.h"
#include "xtal.h"

void btns_init(void) {
  //TRISB =  botoes, entrada
  //WPUB = HABILITA PULL-UPS (PORTB)
  //IOCB = interrupcoes PORTB
    
  //PORTB = 0;
  BTN_START_TRIS = BTN_STOP_TRIS = BTN_UP_TRIS = BTN_DOWN_TRIS = 1;
  BTN_START_PIN = BTN_STOP_PIN = BTN_UP_PIN = BTN_DOWN_PIN = 0;
    
  #ifdef _HARDWARE_NOVO_
    OPTION_REGbits.nRBPU = 0;  //HABILITA PULL-UPS (BIT ~WBPU)
    WPUB = 0xf0;    // weak pull-up portb
    
    //DESABILITA ANALoGICAS 11 a 13
    //obs: nao funciona se deixar ans12 como 0
    //ANSELH = 0b00000111;
    ANSELH = 0b00010010;
    IOCB = 0xf0;    // interrupt on change PORTB

    INTCONbits.RBIE = 1; // ativa interrupcao PORTB
  #endif //_MODULO_NOVO_
}//btns_init()

TBotao btns_testa(void) {
    if(BTN_START_PIN == 0) {
        return BTN_START;
    }
    if(BTN_STOP_PIN == 0) {
        return BTN_STOP;
    }
    if(BTN_UP_PIN == 0) {
        return BTN_UP;
    }
    if(BTN_DOWN_PIN == 0) {
        return BTN_DOWN;
    }
    __delay_ms(100); //debouncing
    return 0;
}//btns_testa()

#ifdef _HARDWARE_ANTIGO_
TBotao btns_testa_antigo(void) {
    static short int press_start = 0, press_stop = 0, press_up = 0;
    static short int press_down = 0, press_menu=0;

    /********* BOTAO START/SELECT **************/
    if (press_start == 1)
          if(BTN_START_PIN == 0) {
              __delay_ms(10);
              press_start = 0;
              return BTN_START;
          }
    if(BTN_START_PIN == 1) {
        __delay_ms(10);
        press_start = 1;
    }

    /**** BOTAO STOP/ESC *************/
    if (press_stop == 1)
        if(BTN_STOP_PIN == 0) {
            __delay_ms(10);
            press_stop = 0;
            return BTN_STOP;
        }
    if (BTN_STOP_PIN == 1) {
        __delay_ms(10);
        press_stop = 1;
    }

    /**** BOTAO UP *************/
    if (press_up == 1)
        if(BTN_UP_PIN == 0) {
            __delay_ms(10);
            press_up = 0;
            return BTN_UP;
        }
    if (BTN_UP_PIN == 1) {
        __delay_ms(10);
        press_up = 1;
    }

     /**** BOTAO DOWN *************/
    if (press_down == 1)
        if(BTN_DOWN_PIN == 0) {
            __delay_ms(10);
            press_down = 0;
            return BTN_DOWN;
        }
    if (BTN_DOWN_PIN == 1) {
        __delay_ms(10);
        press_down = 1;
    }
    
    /*************************************/    
    return 0;
}//btns_testa_antigo()
#endif