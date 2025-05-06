/*****************************************************************************
 * File:   base_adcon.c
 * Author: alexdg 
 * Comments: 
 * Revision history: 
 * Revisado em 2025-05-05 (alexdg).
 * 
 * O PIC16F886 possui 11 canais anal�gicos 
 * (AN0, AN1, AN2, AN3, AN4, AN8, AN9, AN10, AN11, AN12, AN13), portanto, 
 * 
 * ATEN��O!!! Os canais AN5, AN6 e AN7 n�o est�o dispon�veis.
 * 
 * O array adcon_sensores[] cont�m a defini��o do canal anal�gico utilizado
 * por cada sensor anal�gico (sensor1, sensor2, etc...), conforme a vers�o do hardware.
 *  
 * A numera��o dos sensores come�a em 1 (um), mas o �ndice do vetor adcon_sensores[] come�a em zero.
 * adcon_sensores[0] = canal para o Sensor 1
 * adcon_sensores[1] = canal para o Sensor 2
 * ... 
 * 
 * Distribui��o dos canais anal�gicos nas vers�es 2013 e 2016 do hardware:
 * 
 *                       (  Sensor   )
 * (   Canal   )         (Vers�o Hard)
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
 * 1011  = AN11  = RB4 :   x      D *** Este � o sensor 9 (DHT22) ***
 * 1100  = AN12  = RB0 :   x      5
 * 1101  = AN13  = RB5 :   x      x
 * 1110  = CV REF
 * 1111  = Fixed Ref (0.6 volt fixed reference)
 * 
 * R = VREF+ (tens�o de refer�ncia).
 * D = Pino digital utilizado para o DHT22.
 * - = Canal n�o dispon�vel no PIC16F886.
 * x = Canal n�o utilizado (provavelmente pino est� sendo utilizado para outra funcionalidade).
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
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Variaveis P�blicas ===================================================

//Mat�m o valor da m�nima amostra ocorrida. � inicializada com ADCON_VALOR_MAXIMO_AMOSTRA.
uint16_t adcon_amostra_min;

//Mat�m o valor da maior leitura ocorrida. � inicializada com ADCON_VALOR_MINIMO_AMOSTRA.
uint16_t adcon_amostra_max;

//Mant�m a quantidade de amostras que foram gravadas na EEPROM.
//Obs.: quantidade de amostras = quantidade de aquisi��es * quantidade de sensores.
uint8_t  adcon_quant_amostras_gravadas;

//Tempo entre aquisi��es atualmente configurado.
uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
uint8_t adcon_cfg_quant_sensores_atual;

//Quantidade de sensores da �ltima sess�o MONITORA_GRAVA.
//Isso � importante pois, ap�s ter sido efetuada a gra��o de uma aquisi��o na EEPROM,
//n�o deve ser permitido iniciar outra sess�o MONITORA_GRAVA se adcon_cfg_quant_sensores_atual foi modificado,
//pois neste caso, a quantidade de amostras relativas a uma aquisi��o ficariam inconsistentes em rela��o � quantidade de sensores j� aquisitados.
uint8_t adcon_quant_sensores_aquisitados;

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//Quantidade de leituras a serem feitas em um sensor para calcular a m�dia 
//e gerar o valor da amostra do sensor.
//Em 2024-10-13 trocado de 32 para 1 para testar nos sensores MQ.
#define ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA 1

//Canais onde est�o os sensores anal�gicos:
#if defined(_HARDWARE_2013_)
static const uint8_t adcon_canais[ADCON_QUANT_MAX_SENSORES] = {
    0, //Sensor 1 canal 0.
    1, //Sensor 2 canal 1.
    2, //Sensor 3 canal 2.
    4  //Sensor 4 canal 4. N�o colocar v�rgula no �ltimo.
};

#elif defined(_HARDWARE_2016_)

//Canais de   0 a  13 especificam um canal anal�gico. 
//Canais de 101 a 128 especificam um pino f�sico (1 a 28) do microcontrolador.
typedef struct {
  TAdconModeloSensor modelo; //Codigo do sensor (1 a 255).
  uint8_t            canal;  //Canal ou pino f�sico.
}TAdconSensor;

//Os canais ficaram fora de ordem por uma melhor conveni�ncia de conex�es no hardware.
static const TAdconSensor adcon_sensores[ADCON_QUANT_MAX_SENSORES] = {
    SENS_MQ,      1, //Sensor 1 canal 1.
    SENS_MQ,      0, //Sensor 2 canal 0.
    SENS_MQ,      4, //Sensor 3 canal 4.
    SENS_MQ,      2, //Sensor 4 canal 2.
    SENS_MQ,     12, //Sensor 5 canal 12.
    SENS_MQ,     10, //Sensor 6 canal 10.
    SENS_MQ,      8, //Sensor 7 canal 8.
    SENS_MQ7,     3, //Sensor 8 canal 3 (pino 5), o controle do aquecimento � feito pelo (pino 6).
    SENS_DHT22, 125  //Sensor 9 (DHT22) est� no pino 25 (canal = 125).
    //N�o colocar v�rgula no final do �ltimo item acima.
};

//Sensor 9 (DHT22), pino 25 (canal=125).

//<<<<<<<<<<<<<<<<<<<<< implementar futuramente.
//Poderia colocar a seguinte restri��o: sensores que precisam de mais de um pino
//devem estar em sequ�ncia na ordem dos pinos do microcontrolador.

#endif

//===== Tipos Privados =======================================================

//typedef struct TAdconSensor <<<<<<<<<<<<<<<< deveria estar aqui

//===== Variaveis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

/* Faz a amostragem de um sensor, gerando o valor da amostra.
 * Se for um sensor anal�gico, essa amostragem consiste em efetuar v�rias leituras na porta anal�gica,
 * conforme indicado pela constante ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA e
 * efetuar a m�dia dos valores dessas leituras, resultando no valor
 * da amostra do sensor.
 * @param num_sensor n�mero do sensor a ser amostrado (come�a em zero).
 * @return valor valor absoluto (bin�rio) amostrado do sensor (m�dia das leituras se for um sensor anal�gico).
 */
uint16_t adcon_amostra_sensor(uint8_t num_sensor) {
  //Cont�r� a soma de todas as leituras, para calcular a m�dia (valor da amostra) no final.
  uint32_t acc = 0;
  TAdconSensor sensor;
  sensor = adcon_sensores[num_sensor];
 
  if ( (sensor.modelo == SENS_LM35) || (sensor.modelo == SENS_MQ) ) {
    ADCON0bits.CHS = sensor.canal;
    //Tem que esperar um tempo (pior caso) ap�s trocar o canal:
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

    //Calcula o valor da amostra (m�dia das leituas).
    acc = acc / ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA;
  }
  
  else if (sensor.modelo == SENS_MQ7) {
    ADCON0bits.CHS = sensor.canal; //<<<<<<<<<<<<<<<< n�o ser� utilizado (precisa rever isto)
    //Aplicar protocolo de aquecimento antes de efetuar a leitura
    //...
  }
  
  else if (sensor.modelo == SENS_DHT22) {
    uint8_t porta = sensor.canal; //<<<<<<<<<<<<<<<< n�o ser� utilizado (precisa rever isto)
    //Utilizar protocolo de leitura da porta digital do DHT22;
    //...
  }

  return (uint16_t)acc;
}//adcon_amostra_sensor()

/* Converte o valor de 10 bits do conversor AD para o valor a ser mostrado no display.
 * Essa convers�o depende do tipo de sensor que est� sendo utilizado. */
void adcon_binario_para_valor(uint16_t binario, char* p_str_valor) {
  sprintf(p_str_valor, "%4d", binario);
  
  //sprintf(p_str_valor, "%d.%d", binario / 10, binario % 10);
  
  //div_t temp_div;
  //temp_div =  div((int16_t)binario, 10);
  //sprintf(p_str_valor, "%d.%d", temp_div.quot, temp_div.rem);
}//adcon_binario_para_valor()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================