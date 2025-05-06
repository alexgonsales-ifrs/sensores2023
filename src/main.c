/* 
 * File:   main.c
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 15:57
 */

//===================== ATEN��O PRAGMA =======================================
// Nota1: as diretivas pragma devem ser incluidas antes de qualquer header.
// No entanto, aqui foi incluido o header "versao.h" pois ele s� cont�m as
// defini��es das macros de vers�o _HARDWARE_NOVO, ANTIGO, etc...
//============================================================================

//Necess�rio para incluir as macros de defini��o de vers�o (HARDWARE_NOVO, ANTIGO, etc...)
#include "versao.h" 

#ifdef _HARDWARE_2013_
  //#pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
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

#endif

#ifdef _HARDWARE_2016_
// PIC16F886 Configuration Bit Settings
// 'C' source line config statements

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#endif

//===== Includes =============================================================

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "xtal.h"
#include "base_timer.h" //timer0_init()

#include "ct_handler.h"
#include "ct_estados.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//Esta fun��o � usada somente para marca��o no Clock Stimulus
//void inicializado3(void){ };

#ifdef __DEBUG
//Se estiver em modo __DEBUG ent�o a sa�da da fun��o printf()
//ser� direcionada para a UART do microcontrolador, pois esta � a �nica forma
//de se visualizar a sa�da do printf().
void putch(char dado) {
  while( ! PIR1bits.TXIF) // wait until the transmitter is ready
    continue;
  TXREG = dado;  // send one character
}
#endif

 /**
 * Ponto de entrada da aplica��o.
 * Configura o Timer0, habilita as interrup��es e entra no loop infinito. 
 * @return EXIT_SUCCESS
 */
int main(void) {

  #if defined(_HARDWARE_2013_)
    TRISA = 0xFF; //Configura a porta A como entrada.
    
    //Configura portas anal�gicas AN4, AN3 (VRef+), AN2, AN1, AN0.  
    ANSEL  = 0b00011111;

      ADCON1bits.PCFG = 0b0001; //< configurado para q AN3 seja o pindo de referencia de tensao.
      
      
    //2023-11-07
    //001 = frequencia de conversao: FOSC/8 (recomendado para 4MHz)
    ADCON1bits.ADCS2 = 0;
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;
  
    ADCON1bits.ADFM = 1; // resultado justificado a direita
    ADCON0bits.ADON = 1; //< liga conversor A/D (poderia ligar mais tarde, s� no momento da aquisi��o).

  #elif defined(_HARDWARE_2016_)
    //PORTA
    //Configura todos pinos como entrada, com exce��o de RA4.
    //RA0=AN0 (entrada)
    //RA1=AN1 (entrada)
    //RA2=AN2 (entrada)
    //RA3=AN3 (entrada)
    //RA4=Sensor8_Aquec (sa�da)
    //RA5=AN4  (entrada)
    //RA6=OSC2 (entrada)
    //RA7=OSC1 (entrada)
    TRISA = 0b11101111;
    
    //PORTB
    //Configura todos os pinos como entrada (mas RB4 funcionar� como entrada e sa�da).
    //RB0=AN12
    //RB1=AN10
    //RB2=AN8
    //RB3=BTN_DOWN
    //RB4=Digital (Sensor9TU ser� entrada e sa�da)
    //RB5=BTN_UP
    //RB6=BTN_START
    //RB7=BTN_STOP
    TRISB = 0b11111111;

    //Para poder habilitar os bits Weak Pull-up (bit ~WBPU do Option register).
    OPTION_REGbits.nRBPU = 0; 
    //Configura weak pull-up para os bits 7,6,5,3 da PORTB:
    WPUB = 0xE8; //Weak Pull-up PortB.

    //Interrupt On Change PORTB. Habilita pinos 7,6,5,4,3:
    //RB4 � o Sensor9TU.
    IOCB = 0b11111000; 
    //Habilita Interrup��es PORTB.
    INTCONbits.RBIE = 1; 

    //PORTC
    //Configura todos pinos como sa�da, com exce��o de R7.
    //RC0=LCD_RS
    //RC1=LCD_ENABLE
    //RC2=LCD_D4
    //RC3=LCD_D7
    //RC4=LCD_D6
    //RC5=LCD_D5
    //RC6=TX (RS232)
    //RC7=RX (RS232)
    TRISC = 0b10000000;
    
    //Configura portas anal�gicas:
    //Digital=0
    //Analog=1
    
    //Configura portas anal�gicas AN4, AN3, AN2, AN1, AN0.  
    ANSEL  = 0b00011111;
    //Configura portas anal�gicas AN12, AN10, AN8.
    ANSELH = 0b00010101;

    //Configura refer�ncia negativa para VSS (GND).
    //VSS = 0
    //Pino VREF- = 1
    ADCON1bits.VCFG1 = 0; 
    
    //Configura refer�ncia positiva:
    //Estava configurado para pino VREF+.
    //Em 2024-10-28 foi trocado para VDD, liberando esta entrada para o Sensor8.
    //VDD = 0.
    //Pino VREF+ = 1.
    ADCON1bits.VCFG0 = 0; //trocado para zero em 2024-10-28.
    
    ADCON0bits.ADCS = 0b01; //01 = Fequencia de conversao = FOSC/8 (recomendado para 4MHz).
    ADCON1bits.ADFM = 1;    //Resultado justificado a direita.
    ADCON0bits.ADON = 1;    //Habilita o conversor A/D.  <<< verificar se pode habilitar no momento da aquisi��o.

  #endif //_HARDWARE_2016_

  timer0_init();
  timer1_init();
  
  INTCONbits.T0IE = 1; //Habilita interrup��o Timer0.
  INTCONbits.GIE  = 1; //Habilita interrupcoes globais.
  
  while (1) {
    #ifdef _HARDWARE_2013_
      //Aqui deveria ser implementada a leitura dos bot�es.
      //Atualmente est� implementado no handler, ao ativar o Timer0.
      //N�o � o local correto, mas foi deixado assim.
    
      //TBotao option;
      //Este microcontrolador tem 8 n�veis de stack mas este c�digo pode alcan�ar at� 12 niveis.
      //INTCONbits.GIE = 0; //Desabilita interrupcoes globais para n�o causar stack "overflow".
      //if (hand_rcif) {
        //hand_rcif = 0;
        //prot_rs232_executa();
     // }
      //option = btns_testa_antigo();
      //if (option != 0) {
        //est_maquina(option);
      //}
      //INTCONbits.GIE = 1; //Habilita interrupcoes globais.
      //__delay_us(20);
    #endif

    //=======================================================================
    if (hand_flag_botao) {
      TBotao btn = btns_testa();
      //Considera apenas o pressionamento do botao,
      //desconsiderando o "soltar" do bot�o.
      if (btn != BTN_NULL) {
        est_evento_botao(btn);
        //Para indicar que interrup��o foi tratada.
        hand_flag_botao = 0;
      }
    }//if (hand_flag_botao)
    
    //=======================================================================
    if (hand_flag_timer0){
      est_evento_timer0();
      //Para indicar que interrup��o foi tratada.
      hand_flag_timer0 = 0;
    }//if (hand_flag_timer0)
    
    //=======================================================================
    if (hand_flag_rs232) {
      //<<<<<<<<<<<<<<<<< remover, s� para testar.
        //prot_rs232_executa();
    }//if (hand_flag_rs232)
  
  }//while (1))

  return (EXIT_SUCCESS);
  
  while(1);
}//main()

//===== Final do Arquivo =====================================================