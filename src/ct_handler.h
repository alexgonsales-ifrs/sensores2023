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
    
//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Constantes Públicas ==================================================
    
//===== Tipos Públicos =======================================================
    
//===== Variaveis Públicas ===================================================

extern volatile uint8_t hand_flag_botao;
extern volatile uint8_t hand_flag_timer0;
extern volatile uint8_t hand_flag_rs232;

extern volatile uint8_t hand_vbuffer_rs232[];
extern volatile uint8_t hand_qt_buffer_rs232;

//===== Funcoes Públicas =====================================================

//============================================================================

#ifdef	__cplusplus
}
#endif

#endif	/* CT_HANDLER_H */

