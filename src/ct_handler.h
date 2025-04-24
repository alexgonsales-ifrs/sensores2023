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

//===== Constantes P�blicas ==================================================
    
//===== Tipos P�blicos =======================================================
    
//===== Variaveis P�blicas ===================================================

extern volatile uint8_t hand_flag;
extern volatile TBotao  hand_botao_pressionado;

//===== Funcoes P�blicas =====================================================

//============================================================================

#ifdef	__cplusplus
}
#endif

#endif	/* CT_HANDLER_H */

