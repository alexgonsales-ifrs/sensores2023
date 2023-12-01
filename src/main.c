/* 
 * File:   main.c
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 15:57
 */
#ifdef _MODULO_NOVO_
//  #pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

#ifdef _MODULO_ANTIGO_
  //#pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif


// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "versao.h"
#include "xtal.h"
#include "adcon.h"
#include "botoes.h"
#include "lcd.h"
#include "rs232.h"
#include "timer.h"
#include "menu_principal.h"
#include "serv_eeprom.h"
#include "estados.h"

#include "serv_rs232.h"
#include "prot_rs232.h"

#include "handler.h"

//Esta função é usada somente para marcação no Clock Stimulus
//void inicializado3(void){ };

#ifdef __DEBUG
//Esta macro é implementada somente em modo __DEBUG 
//para que a saída do printf() seja na UART do simulador.
void putch(char dado) {
  while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
    continue;
  TXREG = dado;                     // send one character
}
#endif

 /**
 * Funcao principal: inicializa os dispositivos do PIC, inicializa menus, 
 * configura a interrupcao global, etc...
 * e fica em while().
 * @return EXIT_SUCCESS
 */
int main(void) {

  rs232_init();
  lcd_init();
  
  timer0_init();
  INTCONbits.GIE = 1; //Habilita interrupcoes globais.
  INTCONbits.T0IE = 1;
  
    
  while (1) {
    #ifdef _MODULO_ANTIGO_
      TBotao option;
      //Este microcontrolador tem 8 níveis de stack mas este código pode alcançar até 12 niveis.
      //INTCONbits.GIE = 0; //Desabilita interrupcoes globais para não causar stack "overflow".
      //if (hand_rcif) {
        //hand_rcif = 0;
        //prot_rs232_executa();
     // }
      
      //option = btns_testa_antigo();
      //if (option != 0) {
      //  est_maquina(option);
      //}
      //INTCONbits.GIE = 1; //Habilita interrupcoes globais.
      //__delay_us(20);
    #endif
  }//while (1))
  return (EXIT_SUCCESS);
   
  
  while(1);
}//main()
