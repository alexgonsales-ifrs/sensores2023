/* 
 * File:   botoes.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 11:33
 */

#ifndef BOTOES_H
  #define	BOTOES_H

  #ifdef	__cplusplus
    extern "C" {
  #endif

  #include "versao.h"

  typedef enum {
        BTN_NULL,
        BTN_START,
        BTN_STOP,
        BTN_UP,
        BTN_DOWN,
  } TBotao;
    
  //valores de retorno dos botoes
  //nao usar zero, pois a funcao btns_testa retorna 0 por default
  ///#define BTN_START     1
  //#define BTN_STOP      2
  //#define BTN_UP        3
  //#define BTN_DOWN      4

  /****************************************/
#if defined(_MODULO_NOVO_)
    #define BTN_START_PIN   RB4   //Botão Start/Enter/Select
    #define BTN_STOP_PIN    RB5   //Botão Stop/Exit/Esc
    #define BTN_UP_PIN      RB6   //Botão Up
    #define BTN_DOWN_PIN    RB7   //Botão Down

    #define BTN_START_TRIS  TRISB4
    #define BTN_STOP_TRIS   TRISB5
    #define BTN_UP_TRIS     TRISB6
    #define BTN_DOWN_TRIS   TRISB7
  /*
   Testa qual botão foi pressionado no equipamento e retorna P_START, P_STOP, P_UP, P_DWON
   * ou retorna zero se foi algum outro evento 
   */
  extern TBotao btns_testa(void);
    
#elif defined( _MODULO_ANTIGO_)
    #define BTN_START_PIN       RC3
    #define BTN_START_TRIS      TRISC3

    #define BTN_STOP_PIN        RC2
    #define BTN_STOP_TRIS       TRISC2

    #define BTN_UP_PIN          RC1
    #define BTN_UP_TRIS         TRISC1

    #define BTN_DOWN_PIN        RC0
    #define BTN_DOWN_TRIS       TRISC0

   extern TBotao btns_testa_antigo(void);
   
#endif 

  /**
  * Funcao que inicializa a porta B, configurando para ativar as interrupcoes
  * conforme os pinos onde instalamos os botoes.
  */
  extern void    btns_init(void);
  
    
  #ifdef	__cplusplus
  }
  #endif

#endif	/* BOTOES_H */

