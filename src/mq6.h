/* 
 * File:   mq6.h
 * Author: ti
 *
 * Created on 6 de Janeiro de 2015, 22:27
 */

#ifndef MQ6_H
#define	MQ6_H

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
    
extern uint16_t mq6_calibrar(void);
extern void mq_mostra(uint16_t t_int, uint8_t i);


#ifdef	__cplusplus
}
#endif

#endif	/* MQ6_H */

