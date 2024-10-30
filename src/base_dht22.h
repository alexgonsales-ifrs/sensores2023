/* 
 * File:   base_dht22.h
 * Author: alexdg
 *
 * Created on 28 de Outubro de 2024, 08:54
 */

#ifndef BASE_DHT22_H
#define	BASE_DHT22_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include <xc.h>   //TRISB4
#include "xtal.h" //delay_ms())
    
//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Constantes Públicas ==================================================
    
//Porta onde está conectado o sensor.
//e bit para configurar a porta.
#define BASE_DHT22_PIN  RB4
#define BASE_DHT22_TRIS TRISB4
    
//===== Tipos Públicos =======================================================

//===== Variaveis Públicas ===================================================

extern uint16_t base_dht22_amostra_umidade;
extern uint16_t base_dht22_amostra_temperatura;

//===== Funcoes Públicas =====================================================
char base_dht22_amostra(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE_DHT22_H */

