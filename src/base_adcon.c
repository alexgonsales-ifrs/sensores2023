/*****************************************************************************
 * File:   base_adcon.c
 * Author: alexdg
 * Comments:
 * 
 * O array adcon_canais[] contém a definição do canal analógico utilizado
 * por cada sensor analógico (sensor1, sensor2, etc...), conforme a versão do hardware.
 *  
 * A numeração dos sensores começa em 1 (um), mas o índice do vetor adcon_canais[] começa em zero.
 * adcon_canais[0] = canal para o Sensor 1
 * adcon_canais[1] = canal para o Sensor 2
 * ...
 * adcon_canais[ADCON_CFG_QUANT_MAX_SENSORES_ANALOGICOS-1] = canal para o Sensor N.
 *  
 * Versão Hard =>   2013   2016
 * Canal - Pino -   Sensor Sensor
 * 0000  - AN0  -   1      2
 * 0001  - AN1  -   2      1
 * 0010  - AN2  -   3      4
 * 0011  - AN3  -   x      x
 * 0100  - AN4  -   4      3
 * 0101  - AN5  -   x      x
 * 0110  - AN6  -   x      x
 * 0111  - AN7  -   x      x
 * 1000  - AN8  -   x      7
 * 1001  - AN9  -   x      x
 * 1010  - AN10 -   x      6
 * 1011  - AN11 -   x      8
 * 1100  - AN12 -   x      5
 * 1101  - AN13 -   x      x
 * 1110  - CV REF
 * 1111  - Fixed Ref (0.6 volt fixed reference)
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
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variaveis Públicas ===================================================

//Matém o valor da mínima amostra ocorrida. É inicializada com ADCON_VALOR_MAXIMO_AMOSTRA.
uint16_t adcon_amostra_min;

//Matém o valor da maior leitura ocorrida. É inicializada com ADCON_VALOR_MINIMO_AMOSTRA.
uint16_t adcon_amostra_max;

//Mantém a quantidade de amostras que foram gravadas na EEPROM.
uint8_t  adcon_quant_amostras_gravadas;

//Temo entre aquisições atualmente configurado.
uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
uint8_t adcon_cfg_quant_sensores_atual;

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//Quantidade de leituras a serem feitas em um sensor para calcular a média 
//e gerar o valor da amostra do sensor.
//Em 2024-10-13 trocado de 32 para 1 para testar nos sensores MQ.
#define ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA 1

//Canais onde estão os sensores analógicos:
#if defined(_HARDWARE_2013_)
static const uint8_t adcon_canais[ADCON_CFG_QUANT_MAX_SENSORES_ANALOGICOS] = {
    0, //Sensor 1 canal 0.
    1, //Sensor 2 canal 1.
    2, //Sensor 3 canal 2.
    4  //Sensor 4 canal 4. Não colocar vírgula no último.
};

#elif defined(_HARDWARE_2016_)
//Os canais ficaram fora de ordem por uma melhor conveniência de conexões no hardware.
static const uint8_t adcon_canais[ADCON_CFG_QUANT_MAX_SENSORES_ANALOGICOS] = {
    1,  //Sensor 1 canal 1.
    0,  //Sensor 2 canal 0.
    4,  //Sensor 3 canal 4.
    2,  //Sensor 4 canal 2.
    12, //Sensor 5 canal 12.
    10, //Sensor 6 canal 10.
    8,  //Sensor 7 canal 8.
    11  //Sensor 8 canal 11. Não colocar vírgula no último.
};
#endif

//===== Tipos Privados =======================================================

//===== Variaveis Privadas ===================================================

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

uint16_t adcon_amostra_sensor(uint8_t num_sensor) {
  //Contérá a soma de todas as leituras, para calcular a média (valor da amostra) no final.
  uint32_t acc = 0;
  //Seleciona o canal analógico de onde será feita a leitura:
  ADCON0bits.CHS = adcon_canais[num_sensor]; 
  //Tem que esperar um tempo (pior caso) após trocar o canal:
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

  //Calcula o valor da amostra (média das leituas).
  return (uint16_t)(acc / ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA);
}//adcon_amostra_sensor()

/* Converte o valor de 10 bits do conversor AD para o valor a ser mostrado no display.
 * Essa conversão depende do tipo de sensor que está sendo utilizado.
 */
void adcon_binario_para_valor(uint16_t binario, char* p_str_valor) {
  sprintf(p_str_valor, "%4d", binario);
  
  //sprintf(p_str_valor, "Max=%d.%d", binario / 10, binario % 10);
  
  //div_t temp_div;
  //temp_div =  div((int16_t)valor_amostra, 10);
  //sprintf(str_valor, "%d.%d", temp_div.quot, temp_div.rem);
}

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================