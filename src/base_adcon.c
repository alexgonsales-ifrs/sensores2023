/*****************************************************************************
 * File:   base_adcon.c
 * Author: alexdg 
 * Comments: 
 * Revision history: 
 * Revisado em 2025-05-05 (alexdg).
 * 
 * O PIC16F886 possui 11 canais analógicos 
 * (AN0, AN1, AN2, AN3, AN4, AN8, AN9, AN10, AN11, AN12, AN13), portanto, 
 * 
 * ATENÇÃO!!! Os canais AN5, AN6 e AN7 não estão disponíveis.
 * 
 * O array adcon_sensores[] contém a definição do canal analógico utilizado
 * por cada sensor analógico (sensor1, sensor2, etc...), conforme a versão do hardware.
 *  
 * A numeração dos sensores começa em 1 (um), mas o índice do vetor adcon_sensores[] começa em zero.
 * adcon_sensores[0] = canal para o Sensor 1
 * adcon_sensores[1] = canal para o Sensor 2
 * ... 
 * 
 * Distribuição dos canais analógicos nas versões 2013 e 2016 do hardware:
 * 
 *                       (  Sensor   )
 * (   Canal   )         (Versão Hard)
 * (bin  = nome) = Pino: (2013   2016)
 * 0000  = AN0   = RA0 :   1      2
 * 0001  = AN1   = RA1 :   2      1
 * 0010  = AN2   = RA2 :   3      4
 * 0011  = AN3   = RA3 :   R      8         
 * 0100  = AN4   = RA5 :   4      3  *** Cuidado AN4 = RA5 ***
 * 0101  = AN5   =  -  :   -      -
 * 0110  = AN6   =  -  :   -      -
 * 0111  = AN7   =  -  :   -      -
 * 1000  = AN8   = RB2 :   x      7
 * 1001  = AN9   = RB3 :   x      x
 * 1010  = AN10  = RB1 :   x      6
 * 1011  = AN11  = RB4 :   x      D *** Este é o sensor 9 (DHT22) ***
 * 1100  = AN12  = RB0 :   x      5
 * 1101  = AN13  = RB5 :   x      x
 * 1110  = CV REF
 * 1111  = Fixed Ref (0.6 volt fixed reference)
 * 
 * R = VREF+ (tensão de referência).
 * D = Pino digital utilizado para o DHT22.
 * - = Canal não disponível no PIC16F886.
 * x = Canal não utilizado (provavelmente pino está sendo utilizado para outra funcionalidade).
 * 
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
//Obs.: quantidade de amostras = quantidade de aquisições * quantidade de sensores.
uint8_t  adcon_quant_amostras_gravadas;

//Tempo entre aquisições atualmente configurado.
uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
uint8_t adcon_cfg_quant_sensores_atual;

//Quantidade de sensores da última sessão MONITORA_GRAVA.
//Isso é importante pois, após ter sido efetuada a gração de uma aquisição na EEPROM,
//não deve ser permitido iniciar outra sessão MONITORA_GRAVA se adcon_cfg_quant_sensores_atual foi modificado,
//pois neste caso, a quantidade de amostras relativas a uma aquisição ficariam inconsistentes em relação à quantidade de sensores já aquisitados.
uint8_t adcon_quant_sensores_aquisitados;

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//Quantidade de leituras a serem feitas em um sensor para calcular a média 
//e gerar o valor da amostra do sensor.
//Em 2024-10-13 trocado de 32 para 1 para testar nos sensores MQ.
#define ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA 1

//Canais onde estão os sensores analógicos:
#if defined(_HARDWARE_2013_)
static const uint8_t adcon_canais[ADCON_QUANT_MAX_SENSORES] = {
    0, //Sensor 1 canal 0.
    1, //Sensor 2 canal 1.
    2, //Sensor 3 canal 2.
    4  //Sensor 4 canal 4. Não colocar vírgula no último.
};

#elif defined(_HARDWARE_2016_)

//Canais de   0 a  13 especificam um canal analógico. 
//Canais de 101 a 128 especificam um pino físico (1 a 28) do microcontrolador.
typedef struct {
  TAdconModeloSensor modelo; //Codigo do sensor (1 a 255).
  uint8_t            canal;  //Canal ou pino físico.
}TAdconSensor;

//Os canais ficaram fora de ordem por uma melhor conveniência de conexões no hardware.
static const TAdconSensor adcon_sensores[ADCON_QUANT_MAX_SENSORES] = {
    SENS_MQ,      1, //Sensor 1 canal 1.
    SENS_MQ,      0, //Sensor 2 canal 0.
    SENS_MQ,      4, //Sensor 3 canal 4.
    SENS_MQ,      2, //Sensor 4 canal 2.
    SENS_MQ,     12, //Sensor 5 canal 12.
    SENS_MQ,     10, //Sensor 6 canal 10.
    SENS_MQ,      8, //Sensor 7 canal 8.
    SENS_MQ7,     3, //Sensor 8 canal 3 (pino 5), o controle do aquecimento é feito pelo (pino 6).
    SENS_DHT22, 125  //Sensor 9 (DHT22) está no pino 25 (canal = 125).
    //Não colocar vírgula no final do último item acima.
};

//Sensor 9 (DHT22), pino 25 (canal=125).

//<<<<<<<<<<<<<<<<<<<<< implementar futuramente.
//Poderia colocar a seguinte restrição: sensores que precisam de mais de um pino
//devem estar em sequência na ordem dos pinos do microcontrolador.

#endif

//===== Tipos Privados =======================================================

//typedef struct TAdconSensor <<<<<<<<<<<<<<<< deveria estar aqui

//===== Variaveis Privadas ===================================================

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

/* Faz a amostragem de um sensor, gerando o valor da amostra.
 * Se for um sensor analógico, essa amostragem consiste em efetuar várias leituras na porta analógica,
 * conforme indicado pela constante ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA e
 * efetuar a média dos valores dessas leituras, resultando no valor
 * da amostra do sensor.
 * @param num_sensor número do sensor a ser amostrado (começa em zero).
 * @return valor valor absoluto (binário) amostrado do sensor (média das leituras se for um sensor analógico).
 */
uint16_t adcon_amostra_sensor(uint8_t num_sensor) {
  //Contérá a soma de todas as leituras, para calcular a média (valor da amostra) no final.
  uint32_t acc = 0;
  TAdconSensor sensor;
  sensor = adcon_sensores[num_sensor];
 
  if ( (sensor.modelo == SENS_LM35) || (sensor.modelo == SENS_MQ) ) {
    ADCON0bits.CHS = sensor.canal;
    //Tem que esperar um tempo (pior caso) após trocar o canal:
    __delay_us(20);  

    //Faz n leituras conforme definido em ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA.
    for (uint16_t n = 0; n < ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA; n++) {
      ADCON0bits.GO_nDONE = 1;
      //aguarda o ADC terminar a conversao.
      while (ADCON0bits.GO_nDONE) {
        //Loop vazio.  
      }
      //Acumula o valor da leitura.
      acc += (uint32_t)((ADRESH << 8) + ADRESL); //10bits
    }//for

    //Calcula o valor da amostra (média das leituas).
    acc = acc / ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA;
  }
  
  else if (sensor.modelo == SENS_MQ7) {
    ADCON0bits.CHS = sensor.canal; //<<<<<<<<<<<<<<<< não será utilizado (precisa rever isto)
    //Aplicar protocolo de aquecimento antes de efetuar a leitura
    //...
  }
  
  else if (sensor.modelo == SENS_DHT22) {
    uint8_t porta = sensor.canal; //<<<<<<<<<<<<<<<< não será utilizado (precisa rever isto)
    //Utilizar protocolo de leitura da porta digital do DHT22;
    //...
  }

  return (uint16_t)acc;
}//adcon_amostra_sensor()

/* Converte o valor de 10 bits do conversor AD para o valor a ser mostrado no display.
 * Essa conversão depende do tipo de sensor que está sendo utilizado. */
void adcon_binario_para_valor(uint16_t binario, char* p_str_valor) {
  sprintf(p_str_valor, "%4d", binario);
  
  //sprintf(p_str_valor, "%d.%d", binario / 10, binario % 10);
  
  //div_t temp_div;
  //temp_div =  div((int16_t)binario, 10);
  //sprintf(p_str_valor, "%d.%d", temp_div.quot, temp_div.rem);
}//adcon_binario_para_valor()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================