/*****************************************************************************
 * File:   utils.h
 * Author: alex
 * Comments:
 * 
 * Created on 17 de Agosto de 2025, 20:00
 * 
 * Revision history: 
 * 
 ****************************************************************************/

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//===== Includes =============================================================
#include <stdint.h>
    
//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================

//===== Tipos P�blicos =======================================================

//===== Variaveis P�blicas ===================================================

//===== Funcoes P�blicas =====================================================

extern void util_uint16_to_str(uint16_t binario, char* p_str_valor);
extern void util_uint8_to_strhex(uint8_t binario, char* p_str_valor);

#ifdef	__cplusplus
}
#endif
#endif	/* UTILS_H */
