/* 
 * File:   base_botoes.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 11:33
 */

#ifndef BASE_BOTOES_H
  #define BASE_BOTOES_H

  #ifdef	__cplusplus
    extern "C" {
  #endif

//===== Includes =============================================================
 
  #include "versao.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Tipos P�blicos =======================================================
        
  //Valor de retorno para a fun��o btns_testa().
  typedef enum {
        BTN_NULL,
        BTN_START,
        BTN_STOP,
        BTN_UP,
        BTN_DOWN,
  } TBotao;
    
//===== Constantes P�blicas ==================================================

#if defined(_HARDWARE_2013_)
    #define BTN_START_PIN       RC3
    #define BTN_STOP_PIN        RC2
    #define BTN_UP_PIN          RC1
    #define BTN_DOWN_PIN        RC0
#elif defined(_HARDWARE_2016_)
    //Est�o definidos fora de ordem por uma conveni�ncia de conex�o na
    //placa do hardware.
    #define BTN_START_PIN   RB6   //Bot�o Start/Enter/Select
    #define BTN_STOP_PIN    RB7   //Bot�o Stop/Exit/Esc
    #define BTN_UP_PIN      RB5   //Bot�o Up
    #define BTN_DOWN_PIN    RB3   //Bot�o Down
#endif
  
//===== Funcoes P�blicas =====================================================
  
extern void btns_init(void);

/* Testa qual bot�o foi pressionado no equipamento e retorna o bot�o (TBotao). */
extern TBotao btns_testa(void);
    
  #ifdef	__cplusplus
  }
  #endif

#endif /* BASE_BOTOES_H */

