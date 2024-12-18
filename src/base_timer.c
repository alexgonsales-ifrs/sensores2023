/*****************************************************************************
 * File:   base_timer.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <xc.h>
//#include <htc.h>

//#include <pic16f886.h>
//#include <proc/pic16f876a.h>

#include "base_timer.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Variaveis P�blicas ===================================================

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variaveis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================


//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================


void timer0_init(void) {

    /** TMR0 = 39\n
     * Frequencia de 4 MHz, entao 1 us e o ciclo de instrucao. Para gerar
     * um atraso de 1 segundo, sao necessarias 1.000.000 de ciclos.
     * Como o prescaler esta setado para 256, entao
     * Freq Timer 0 = 1M Hz / 256 = 3906,25 Hz.
     * Periodo do Timer 0 =  1 / 3906,25Hz = 0,000256s ou 0,256ms ou 256us
     * Fazendo TMR0 = 39,
     * 256-39 = 217.
     * 0,000256s x 217 = 0,055552s.
     * 0,05555 x 18 = 0,999936s.   3906,25/217 = 18,00115.
     * Temos que ter 18 interrupcoes para chegar a 1 segundo.
     */
    TMR0 = 39;
    TMR1L = 0;  //<<<<<<<<<<<<<<<<<<<<<<< retirar

    OPTION_REGbits.T0CS = 0; // timer = Fosc/4
    OPTION_REGbits.PSA = 0; // prescaler -> timer0 em vez de wdt
    OPTION_REGbits.PS = 0b111; // prescaler 1:256
    
    //T0CS = 0; // timer = Fosc/4
    //PSA = 0; // prescaler -> timer0 em vez de wdt
    //PS = 0b111; // prescaler 1:256

    
    //T0CS_bit = 0; // timer = Fosc/4
    //PSA_bit = 0; // prescaler -> timer0 em vez de wdt
    //PS = 0b111; // prescaler 1:256

    //INTCONbits.T0IF = 0; //limpa flag interrupcao do timer 0
    //INTCONbits.T0IE = 0; //desabilita interrupcao do timer 0
    
    T0IF = 0; //limpa flag interrupcao do timer 0
    T0IE = 0; //desabilita interrupcao do timer 0

}//timer0_init()

void timer1_init(void) {
    TMR1H = 0; //<<<<<<<<<<<<<<<<<<<<<<< retirar
    TMR1L = 0; //<<<<<<<<<<<<<<<<<<<<<<< retirar

    T1CONbits.TMR1CS = 0; //Internall clock (Fosc/4).
    T1CONbits.TMR1ON = 0; //Desliga o Timer.

    PIE1bits.TMR1IE = 0; //desabilita interrupcao do timer 1
    PIR1bits.TMR1IF = 0; //limpa flag interrupcao do timer 1

}//timer1_init()

//void timer2_init(void) {
/*    TMR2 = 39;
    PR2 = 0xff; 
    // timer2 e Fosc/4
    T2CONbits.T2CKPS = 3;   // prescaler 1:16
    T2CONbits.TOUTPS = 0xf; // postscaler 1:16
    INTCONbits.PEIE = 1;    // Precisa ser habilitado para TIMER2 funcionar
    PIE1bits.TMR2IE = 1;    // Habilita interrupcao do TIMER2
 */
//}

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
