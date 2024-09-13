/* 
 * File:   base_timer.h
 * Author: ti
 *
 * Created on 2 de Janeiro de 2014, 11:05
 */

#ifndef BASE_TIMER_H
#define	BASE_TIMER_H

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

//===== Funcoes Públicas =====================================================
    
/**
* Inicializa o timer 0. E chamada durante a funcao main().
*/
extern void timer0_init(void);
//void timer2_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE_TIMER_H */

