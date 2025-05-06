/* 
 * File:   base_dht22.h
 * Author: alexdg
 * Created on 28 de Outubro de 2024, 08:54
 * Revision history: 
 * Revisado em 2025-05-05 (alexdg).
 * 
 */

#ifndef BASE_DHT22_H
#define	BASE_DHT22_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include <xc.h>   //TRISB4
    
//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================
    
//Porta onde est� conectado o sensor.
//e bit para configurar a porta.
#define BASE_DHT22_PIN  RB4
#define BASE_DHT22_TRIS TRISB4
    
//===== Tipos P�blicos =======================================================

//===== Variaveis P�blicas ===================================================

//Cont�m o valor da umidade lida pelo sensor.
//High Byte (8 bits): parte inteira da umidade.
//Low  Byte (8 bits): parte decimal da umidade.
extern uint16_t base_dht22_amostra_umidade;

//Cont�m o valor da temperatura lida pelo sensor.
//High Byte (8 bits): parte inteira da temperatura.
//Low  Byte (8 bits): parte decimal da temperatura.
extern uint16_t base_dht22_amostra_temperatura;

//===== Funcoes P�blicas =====================================================
extern char base_dht22_amostra(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE_DHT22_H */

