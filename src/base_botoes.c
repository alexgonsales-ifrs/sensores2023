/*****************************************************************************
 * File:   base_botoes.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <xc.h>
#include "base_botoes.h"
#include "versao.h"
#include "xtal.h"

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

/*
 Inicializa vari�veis dos bot�es.
 */
void btns_init(void) {
  #if defined(_HARDWARE_2013_)

  #elif defined(_HARDWARE_2016_)
    //s�o pinos do PIC, n�o precisa inicializar
    //BTN_START_PIN = BTN_STOP_PIN = BTN_UP_PIN = BTN_DOWN_PIN = 1;
  #endif
}//btns_init()

/****************************************************************************
 * Testa qual bot�o foi pressionado no equipamento e retorna o bot�o (TBotao).
 * 
 ***************************************************************************/
TBotao btns_testa(void) {
  TBotao btn_aux;
  btn_aux = BTN_NULL;
  
  #if defined(_HARDWARE_2013_)

    static short int press_start = 0, press_stop = 0, press_up = 0;
    static short int press_down = 0, press_menu=0;

    /********* BOTAO START/SELECT **************/
    if (press_start == 1)
          if(BTN_PIN_START == 0) {
              __delay_ms(10);
              press_start = 0;
              return BTN_START;
          }
    if(BTN_PIN_START == 1) {
        __delay_ms(10);
        press_start = 1;
    }

    /**** BOTAO STOP/ESC *************/
    if (press_stop == 1)
        if(BTN_PIN_STOP == 0) {
            __delay_ms(10);
            press_stop = 0;
            return BTN_STOP;
        }
    if (BTN_PIN_STOP == 1) {
        __delay_ms(10);
        press_stop = 1;
    }

    /**** BOTAO UP *************/
    if (press_up == 1)
        if(BTN_PIN_UP == 0) {
            __delay_ms(10);
            press_up = 0;
            return BTN_UP;
        }
    if (BTN_PIN_UP == 1) {
        __delay_ms(10);
        press_up = 1;
    }

     /**** BOTAO DOWN *************/
    if (press_down == 1)
        if(BTN_PIN_DOWN == 0) {
            __delay_ms(10);
            press_down = 0;
            return BTN_DOWN;
        }
    if (BTN_PIN_DOWN == 1) {
        __delay_ms(10);
        press_down = 1;
    }
    
    /*************************************/    
    return 0;
  
  #elif defined(_HARDWARE_2016_)
    
    if(BTN_PIN_START == 0) {
        btn_aux = BTN_START;
    }
    if(BTN_PIN_STOP == 0) {
        btn_aux = BTN_STOP;
    }
    if(BTN_PIN_UP == 0) {
        btn_aux = BTN_UP;
    }
    if(BTN_PIN_DOWN == 0) {
        btn_aux = BTN_DOWN;
    }
    
    //Debouncing:
    //Efetuado teste no simulador, necessita debouncing de pelo menos 3ms.
    //Foi utilizado 50ms para garantir qualquer bounce (ru�do) na chave (switch) f�sica.
    __delay_ms(50);
    
    //Tem que usar esta vari�vel auxiliar pois tem que executar o delay()
    //antes de excutar o return;
    return btn_aux;
    
  #endif //_HARDWARE_2016_

}//btns_testa()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================