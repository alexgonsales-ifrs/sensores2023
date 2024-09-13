/* 
 * File:   base_rs232.h
 * Author: ti
 *
 * Created on 22 de Janeiro de 2014, 05:10
 */

#ifndef BASE_RS232_H
#define	BASE_RS232_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================

#include <stdint.h>

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.
    
//===== Constantes Públicas ==================================================

//===== Tipos Públicos =======================================================

//===== Variaveis Públicas ===================================================

//===== Funcoes Públicas =====================================================
    
/**
* Inicializa a interface serial. E chamada durante a funcao main().
*/
extern void rs232_init(void);

/**
 * Envia um dado (byte) pela RS232.
 * @param dado o byte a ser enviado pela RS232.
 */
extern void rs232_envia_byte(uint8_t dado);

/**
 * Envia um dado (byte) pela RS232 em formato ASCII (hexadecimal).
 * Converte o byte para uma string que representa o dado em hexadecimal e 
 * envia esses caracteres pela serial.
  * @param dado o byte a ser enviado pela RS232.
 */
extern void rs232_envia_byte_hexa(uint8_t dado);

extern void rs232_envia_string(char* str);


#ifdef	__cplusplus
}
#endif

#endif	/* BASE_RS232_H */

