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
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Tipos Públicos =======================================================
        
  //Valor de retorno para a função btns_testa().
  typedef enum {
        BTN_NULL,
        BTN_START,
        BTN_STOP,
        BTN_UP,
        BTN_DOWN,
  } TBotao;
    
//===== Constantes Públicas ==================================================

#if defined(_HARDWARE_2013_)
    #define BTN_START_PIN       RC3
    #define BTN_STOP_PIN        RC2
    #define BTN_UP_PIN          RC1
    #define BTN_DOWN_PIN        RC0
#elif defined(_HARDWARE_2016_)
    //Estão definidos fora de ordem por uma conveniência de conexão na
    //placa do hardware.
    #define BTN_START_PIN   RB6   //Botão Start/Enter/Select
    #define BTN_STOP_PIN    RB7   //Botão Stop/Exit/Esc
    #define BTN_UP_PIN      RB5   //Botão Up
    #define BTN_DOWN_PIN    RB3   //Botão Down
#endif
  
//===== Funcoes Públicas =====================================================
  
extern void btns_init(void);

/* Testa qual botão foi pressionado no equipamento e retorna o botão (TBotao). */
extern TBotao btns_testa(void);
    
  #ifdef	__cplusplus
  }
  #endif

#endif /* BASE_BOTOES_H */

