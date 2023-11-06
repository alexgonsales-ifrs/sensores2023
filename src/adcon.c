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

#include "adcon.h"
#include "xtal.h"
#include "versao.h"

#ifdef _MQ_
#include "mq6.h"
#endif

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variaveis Públicas ===================================================

//Matém o valor da mínima leitura ocorrida. É inicializada com ADCON_VALOR_MAXIMO_LEITURA.
uint16_t adcon_leitura_min;

//Matém o valor da maior leitura ocorrida. É inicializada com ADCON_VALOR_MINIMO_LEITURA.
uint16_t adcon_leitura_max;

//Mantém a quantidade de leituras que foram gravadas na EEPROM.
uint8_t  adcon_quant_leituras_gravadas;

//Temo entre amostras atualmente configurado.
uint16_t adcon_cfg_tempo_amostra_atual;

//Quantidade de sensores atualmente configurada.
uint8_t adcon_cfg_quant_sensores_atual;

//============================================================================
//===== Definições Privadas ==================================================
//============================================================================

//===== Constantes Privadas ==================================================

//Quantidade de aquisições a serem feitas em um sensor para calcular a média 
//e gerar o valor da leitura do sensor.
#define ADCON_QUANT_AQUISICOES_MEDIA_LEITURA 32

/*
Configuração dos canais do conversor analógico/digital:
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

//Canais em que estão cada um dos 4 sensores:
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

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

/*****************************************************************************
 * Funcao que inicializa o conversor analogico/digital.
 * É chamada na funcao main().
 *****************************************************************************/
void adcon_init(void) {
  TRISA = 0xff;
  #ifdef _PIC16F886_H_
    ANSEL = 0b00011111; /// configura porta como analogica
    //(AN7:5) nao implementadas no PIC16F886
    ADCON1bits.VCFG1 = 0; // Referencia negativo
    // 1 = pino Vref- ; 0 = Vss
    ADCON1bits.VCFG0 = 1; // Referencia positivo
    // 1 = pino Vref+ ; 0 = Vdd
  #endif
  #ifdef _PIC16F876A_H_
    ADCON1bits.PCFG = 0b0001; //< configurado para q o pino seja a referencia de tensao
  #endif
  ADCON0bits.ADCS = 1; // frequencia de conversao: FOSC/8 (recomendado para 4MHz)
  ADCON1bits.ADFM = 1; // resultado justificado a direita
  ADCON0bits.ADON = 1; //< liga conversor A/D
}//adcon_init()

uint16_t adcon_leitura_sensor(uint8_t num_sensor) {
  //Contérá a soma de todas as aquisições, para calcular a média no final.
  uint32_t acc = 0;
  //Seleciona o canal analógico de onde será feita a leitura:
  ADCON0bits.CHS = canais[num_sensor]; 
  //Tem que esperar um tempo (pior caso) após trocar o canal:
  __delay_us(20);  
  
  //Faz n aquisições conforme definido em ADCON_QUANT_AQUISICOES_MEDIA_LEITURA.
  for (uint16_t n = 0; n < ADCON_QUANT_AQUISICOES_MEDIA_LEITURA; n++) {
    ADCON0bits.GO_nDONE = 1;
    //aguarda o ADC terminar a conversao.
    while (ADCON0bits.GO_nDONE) {
      //Loop vazio.  
    }
    //Acumula o valor da aquisição.
    acc += (uint32_t)((ADRESH << 8) + ADRESL); //10bits
  }//for

  //Calcula a média das aquisições:
  return (uint16_t)(acc / ADCON_QUANT_AQUISICOES_MEDIA_LEITURA);
}//adcon_leitura_sensor()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================