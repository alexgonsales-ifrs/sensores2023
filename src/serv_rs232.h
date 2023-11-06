/* 
 * File:   serv_rs232.h
 * Author: Alex
 *
 * Created on 3 de Novembro de 2023, 12:08
 */

#ifndef SERV_RS232_H
#define	SERV_RS232_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include <stdint.h>
    
//===== Constantes ===========================================================
    
//===== Tipos ================================================================

//===== Variáveis ============================================================

//===== Funções ==============================================================

/**
* Envia pela RS232 todas as leituras que estão gravadas na EEPROM.
* @return quantidade de bytes transmitidos
*/
extern uint8_t serv_rs232_envia_leituras_gravadas_eeprom(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_RS232_H */

