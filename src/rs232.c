#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#include "rs232.h"
#include "versao.h"
#include "xtal.h"

void rs232_init(void) {
  #ifdef _PIC16F886_H_
    BRG16 = 0;
  #endif
  TRISC6 = 0; //tx = saida
  TRISC7 = 1; //rx = entrada
  TXEN = 1;
  SYNC = 0;
  SPEN = 1;
  CREN = 1;
  PIE1bits.RCIE = 1;
  INTCONbits.PEIE = 1;   
  BRGH = 0;
  SPBRG = 25; //2400 BPS
  TX9 = 0;
  PIR1bits.RCIF = 0;
}//rs232_init())

/**
 * Envia um dado (byte) pela RS232.
 * @param dado o byte a ser enviado pela RS232.
 */
void rs232_envia_byte(uint8_t dado) {
  TXREG = dado;
  __delay_ms(5);
}//rs232_envia_byte()

/**
 * Envia um dado (byte) pela RS232 em formato ASCII (hexadecimal).
 * Converte o byte para uma string que representa o dado em hexadecimal e 
 * envia esses caracteres pela serial.
  * @param dado o byte a ser enviado pela RS232.
 */
void rs232_envia_byte_hexa(uint8_t dado) {
  char byte_hexa[4];
  char* p_byte_hexa;
  sprintf(byte_hexa, "%02X", dado);
  p_byte_hexa = &byte_hexa[0];
  while (*p_byte_hexa) {
    TXREG = *p_byte_hexa;
    p_byte_hexa++;
    __delay_ms(5);
  }  
}//rs232_envia_byte_hexa()

