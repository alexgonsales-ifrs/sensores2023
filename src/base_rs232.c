/*****************************************************************************
 * File:   base_rs232.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <xc.h>
#include <stdint.h>
//#include <stdio.h>

#include "base_rs232.h"
#include "versao.h"
#include "xtal.h"
#include "utils.h"

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variaveis Públicas ===================================================

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variaveis Privadas ===================================================

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

void rs232_init(void) {

  #ifdef _PIC16F886_H_
    //8-bit Baud Rate Generator is used.
    BRG16 = 0;
  #endif
  
  //Baud Generator
  BAUDCTLbits.BRG16 = 0; //0=disable 16-bit baud generator (uses 8-bit baud generator).
  TXSTAbits.BRGH    = 0; //0=low speed.
  SPBRG             = 25; //Valor para resultar em baud rate 2400Hz para FOSC = 4MHz.
  //SPBRGH          = 0; //Utilizado somente para 16-bit baud rate generator.
  
  //Calcula baud rate para as configurações acima:
  //Baud Rate = FOSC / (64 * (SPBRGH:SPBRG + 1))
  //baud rate =   4M / (64 * (0:25         + 1))
  //baud rate =   4M / (64 * (  25         + 1))
  //baud rate =   4M / (64 *    26             )
  //baud rate =   4M / (1664)
  //baud rate =   2404Hz

//Configuração:
  //SYNC = 0; //Assíncrona.
  //SPEN = 1; //Serial Port Enable: configures RX/DT and TX/CK pins as serial port pins.
  //TXEN = 1; //Transmit enabled.
  //CREN = 1; //Continuous Receive Enable bit. 1=Enables receiver.
  //TX9 = 0;
  
  TXSTAbits.SYNC = 0; //Assíncrona.
  TXSTAbits.TXEN = 1; //Transmit enabled.
  TXSTAbits.TX9  = 0;
  
  RCSTAbits.SPEN = 1; //Serial Port Enable: configures RX/DT and TX/CK pins as serial port pins.
  RCSTAbits.CREN = 1; //Continuous Receive Enable bit. 1=Enables receiver.
  
  //<<<<<<<<<<<<<<<<< remover: habilitar novamente 15/04/2025
  //INTCONbits.PEIE = 1; //Peripheral interrupt enable bit
  //PIE1bits.RCIE = 1;   //Receive interrupt enable bit. Habilita interrupção RX.
  //PIR1bits.RCIF = 0;  //Receive interrupt flag.
  
  //Passado para a funcao main().
  //INTCONbits.PEIE = 1; //Peripheral interrupt enable bit
  //PIE1bits.RCIE = 1;   //Receive interrupt enable bit. Habilita interrupção RX.
  PIR1bits.RCIF = 0;  //Receive interrupt flag.
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
  
  util_uint8_to_strhex(dado, byte_hexa);
  byte_hexa[2] = 'X';
  byte_hexa[3] = 0; //final string.
  
  //sprintf(byte_hexa, "%02X", dado);
  p_byte_hexa = &byte_hexa[0];
  while (*p_byte_hexa) {
    TXREG = *p_byte_hexa;
    p_byte_hexa++;
    __delay_ms(5);
  }  
}//rs232_envia_byte_hexa()

void rs232_envia_string(char* str) {
  uint8_t i =0;
  while (str[i]) {
  TXREG = str[i];
  __delay_ms(5);
  i++;    
  }
}

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

