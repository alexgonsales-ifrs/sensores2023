/* 
 * File:   handler.h
 * Author: alexdg
 *
 * Created on 29 de Setembro de 2023, 11:54
 */

#ifndef CT_HANDLER_H
#define	CT_HANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include "base_botoes.h"
    
//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//typedef enum {HAND_FLAG_NULL, HAND_FLAG_BOTAO, HAND_FLAG_TIMER0, HAND_FLAG_RS232} THandFlag;

//===== Constantes P�blicas ==================================================
    
//===== Tipos P�blicos =======================================================
    
//===== Variaveis P�blicas ===================================================

//extern volatile THandFlag hand_flag;
    
extern volatile uint8_t hand_flag_botao;
extern volatile uint8_t hand_flag_timer0;
extern volatile uint8_t hand_flag_rs232;

extern volatile TBotao  hand_botao_pressionado;

//===== Funcoes P�blicas =====================================================

//============================================================================

#ifdef	__cplusplus
}
#endif

#endif	/* CT_HANDLER_H */

