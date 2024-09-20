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
#include <stdlib.h>
#include <xc.h>
#include <stdio.h>

#include "base_adcon.h"
#include "xtal.h"
#include "versao.h"

#ifdef _MQ_
#include "mq6.h"
#endif

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Variaveis P�blicas ===================================================

//Mat�m o valor da m�nima amostra ocorrida. � inicializada com ADCON_VALOR_MAXIMO_AMOSTRA.
uint16_t adcon_amostra_min;

//Mat�m o valor da maior leitura ocorrida. � inicializada com ADCON_VALOR_MINIMO_AMOSTRA.
uint16_t adcon_amostra_max;

//Mant�m a quantidade de amostras que foram gravadas na EEPROM.
uint8_t  adcon_quant_amostras_gravadas;

//Temo entre aquisi��es atualmente configurado.
uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
uint8_t adcon_cfg_quant_sensores_atual;

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//Quantidade de leituras a serem feitas em um sensor para calcular a m�dia 
//e gerar o valor da amostra do sensor.
#define ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA 32

/*
Configura��o dos canais do conversor anal�gico/digital:
Canal - Pino - Sensor
0000  - AN0  - Sensor 1
0001  - AN1  - Sensor 2
0010  - AN2  - Sensor 3
0011  - AN3
0100  - AN4  - Sensor 4
0101  - AN5
0110  - AN6
0111  - AN7
1000  - AN8
1001  - AN9
1010  - AN10
1011  - AN11
1100  - AN12
1101  - AN13
1110  - CV REF
1111  - Fixed Ref (0.6 volt fixed reference)
*/

//Canais em que est�o cada um dos 4 sensores:
static const uint8_t canais[ADCON_CFG_QUANT_SENSORES_MAX] = {
    0,
    1,
    2,
    4
};

/*versao 8 sensores
const uint8_t canais[CFG_QUANT_SENSORES_MAX] = {
    0,
    1,
    2,
    4,  //ra5
    12, //rb0
    10, //rb1
    8,  //rb2
    9   //rb3
};
*/

//===== Tipos Privados =======================================================

//===== Variaveis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

/*****************************************************************************
 * Funcao que inicializa o conversor analogico/digital.
 * � chamada na funcao main().
 *****************************************************************************/
void adcon_init(void) {
  TRISA = 0xFF; //Configura a porta A como entrada.
  #ifdef _PIC16F886_H_
    ANSEL = 0b00011111; /// configura porta como analogica
    //(AN7:5) nao implementadas no PIC16F886
    ADCON1bits.VCFG1 = 0; // Referencia negativo
    // 1 = pino Vref- ; 0 = Vss
    ADCON1bits.VCFG0 = 1; // Referencia positivo
    // 1 = pino Vref+ ; 0 = Vdd
    
    //ADCON0bits.ADCS = 1; // frequencia de conversao: FOSC/8 (recomendado para 4MHz)
    //ADCON1bits.ADFM = 1; // resultado justificado a direita
    //ADCON0bits.ADON = 1; //< liga conversor A/D

  #endif

  #ifdef _PIC16F876A_H_
    ADCON1bits.PCFG = 0b0001; //< configurado para q AN3 seja o pindo de referencia de tensao.
    //2023-11-07
    //001 = frequencia de conversao: FOSC/8 (recomendado para 4MHz)
    ADCON1bits.ADCS2 = 0;
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;
  
    ADCON1bits.ADFM = 1; // resultado justificado a direita
    ADCON0bits.ADON = 1; //< liga conversor A/D (poderia ligar mais tarde, s� no momento da aquisi��o).
  #endif
  
}//adcon_init()

uint16_t adcon_amostra_sensor(uint8_t num_sensor) {
  //Cont�r� a soma de todas as leituras, para calcular a m�dia (valor da amostra) no final.
  uint32_t acc = 0;
  //Seleciona o canal anal�gico de onde ser� feita a leitura:
  ADCON0bits.CHS = canais[num_sensor]; 
  //Tem que esperar um tempo (pior caso) ap�s trocar o canal:
  __delay_us(20);  
  
  //Faz n leituras conforme definido em ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA.
  for (uint16_t n = 0; n < ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA; n++) {
    ADCON0bits.GO_nDONE = 1;
    //aguarda o ADC terminar a conversao.
    while (ADCON0bits.GO_nDONE) {
      //Loop vazio.  
    }
    //Acumula o valor da leitura.
    acc += (uint32_t)((ADRESH << 8) + ADRESL); //10bits
  }//for

  //Calcula o valor da amostra (m�dia das leituas).
  return (uint16_t)(acc / ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA);
}//adcon_amostra_sensor()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================