/*****************************************************************************
 * File:   adcon.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <stdint.h>
//#include <stdio.h>
#include <string.h>
#include <math.h>
#include <xc.h>

#include "base_adcon.h"
#include "base_lcd.h"
#include "mq6.h"
#include "versao.h"
#include "xtal.h"
#include "utils.h"

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

#define K 19.5897852088
#define INV_N -2.32192809474

    const uint16_t mq_gl_r0 = 378;

//Resistance of sensor(Rs): Rs=(Vc/VRL-1)×RL
    
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
/**
 * Rotina de calibracao do sensor mq6.
 * @return a resistencia R0 de calibracao
 */
uint16_t mq6_calibrar(void) {
    uint16_t vrl, ant;
    vrl = ant = 0;
    //Descobrir R0
    //do {
    //    ant = vrl;
        for (uint8_t i = 0; i < 90; i++) {
            __delay_ms(1000);
        }
        //vrl = adcon_ler_canal(0);  //alexdg: comentado em 2023
    //} while (ant != vrl);

    return vrl;
}

/**
 * Mostra no display o valor t_int convertido para concentracao em ppm.
 * @param t_int valor a ser mostrado
 * @param i posicao no vetor de coordenadas do display
 */
void mq_mostra(uint16_t t_int, uint8_t i) {
    //const double ninv = 1/-0.4306765581;
    //const double ninv = -2.32192809474;
    //const double k = 19.5897852088;
    char temp_str[16] = {0}; //precisa ter tamanho declarado
    char temp_str2[16] = {0};
    //char *temp_str;
    uint16_t ppm;
    double yk;

    /*formula: (y/k)^(1/n)=x
     * ppm = yk ^ (1/n)
     * ppm = (r0 / t *k) ^ (1/n)
     *  */
    yk = (mq_gl_r0 / (t_int * K));
    ppm = (uint16_t)pow(yk, INV_N);
    /*
    sprintf(temp_str, "ppm=%u", pow(yk, ninv)); //resultado errado
     */
    //sprintf(temp_str, "ppm=%u", ppm);
    lcd_clear();
    
    strcpy(temp_str, "ppm=");
    util_uint16_to_str(t_int, &temp_str[4]);
    //sprintf(temp_str, "ppm=%u", t_int);
    
    lcd_goto_sensor(i);
    //lcd_goto(LCD_POSICAO[i].linha, LCD_POSICAO[i].coluna); 
    lcd_puts(temp_str);
    /*
    sprintf(temp_str2,"r0=%d rs=%d", mq_gl_r0, t_int);
    lcd_goto(2, 0);
    lcd_puts(temp_str2);
     */
}
//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
