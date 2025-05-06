/*****************************************************************************
 * File:   base_adcon.h
 * Author: alexdg
 * Comments:
 * Revision history: 
 * Revisado em 2025-05-05 (alexdg).
 *
 * Definições importantes para este contexto:
 * 
 * Leitura: é a leitura de um valor da porta analógica.
 *  
 * Amostra: é a leitura de vários valores da porta analógica, resultando no valor médio, sendo este considerado 
 * como o valor lido (amostrado) do sensor. Por exemplo, uma amostra de um sensor pode corresponder à leitura de 10 valores do sensor.
 * 
 * Aquisição: é um conjunto de amostras de todos os sensores atualmente configurados.
 * Por exemplo, se está configurado para 4 sensores, então uma aquisição corresponde a 4 amostras.
 * Supondo que cada amostra corresponda à média da leitura de 10 valores,
 * então uma aquisição corresponderá à leitura de 40 valores (armazenamento de apenas 4 amostras).
 * 
 * Sessão de monitoramento: corresponde a uma sessão onde são realizadas várias aquisições, 
 * todas com o mesmo intervalo de tempo (tempo entre aquisções), conforme configuração escolhida pelo usuário.
 * 
 ****************************************************************************/

#ifndef BASE_ADCON_H
#define	BASE_ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include "versao.h"
    
//===== Constantes ===========================================================

  //As constantes abaixo são utilizadas para inicialização das variáveis adcon_amostra_min e adcon_amostra_max.
  //São os valores mínimo e máximo possíveis na amostra de um sensor.
  //Apesar do conversor analógico/digital ter resolução de 10 bits, o que daria o valor máximo de 1023,
  //estamos utilizando como valor máximo 65535 = 0xFFFF (máximo valor possível em 16 bits).
  #define ADCON_VALOR_MINIMO_AMOSTRA          0
  #define ADCON_VALOR_MAXIMO_AMOSTRA          0xFFFF

  //Quantidade máxima possível de sensores.
  #if defined(_HARDWARE_2013_)
    #define ADCON_QUANT_MAX_SENSORES 4
  #elif defined(_HARDWARE_2016_)
    //6 sensores MQ.
    #define ADCON_QUANT_MAX_SENSORES 9
  #endif

  //1 sensor digital DHT22 que contará como 2 sensores (temperatura e umidade).
  //No display, as posições 7 e 8 serão reservadas para o sensor DHT22.
  //Posição 7 - Temperatura.
  //Posição 8 - Umidade.
  
  //Quantidade máxima de amostras permitidas (gravadas na EEPROM). 
  //A EEPROM tem 240 bytes disponíveis para isso.
  //Cada amostra ocupa 2 bytes, portanto são possíveis 120 amostras.
  //A quantidade de aquisições dependerá da quantidade de sensores que está configurado.
  //Quantidade máxima de aquisições = QUANT_MAX_AMOSTRAS / QUANT_SENSORES
  #define ADCON_QTD_MAX_AMOSTRAS              120 

  // Valores de selecao do tempo entre aquisições:
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO     18
  #define     ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS   180
  #define     ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS   540
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO      1080
  #define     ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS    10800
  #define     ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS    32400
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_HORA        64800

//===== Tipos ================================================================

typedef enum  {SENS_LM35, SENS_DHT22, SENS_MQ, SENS_MQ7} TAdconModeloSensor;

//===== Variáveis ============================================================

//Matém o valor da mínima amostra ocorrida. É inicializada com ADCON_VALOR_MAXIMO_AMOSTRA.
extern uint16_t adcon_amostra_min;

//Matém o valor da maior leitura ocorrida. É inicializada com ADCON_VALOR_MINIMO_AMOSTRA.
extern uint16_t adcon_amostra_max;

//Mantém a quantidade de amostras que foram gravadas na EEPROM.
//Obs.: quantidade de amostras = quantidade de aquisições * quantidade de sensores.
extern uint8_t  adcon_quant_amostras_gravadas;

//Tempo entre aquisições atualmente configurado.
extern uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
extern uint8_t adcon_cfg_quant_sensores_atual;

//Quantidade de sensores da última sessão MONITORA_GRAVA.
//Isso é importante pois, após ter sido efetuada a gração de uma aquisição na EEPROM,
//não deve ser permitido iniciar outra sessão MONITORA_GRAVA se adcon_cfg_quant_sensores_atual foi modificado,
//pois neste caso, a quantidade de amostras relativas a uma aquisição ficariam inconsistentes em relação à quantidade de sensores já aquisitados.
extern uint8_t adcon_quant_sensores_aquisitados;

//===== Funções ==============================================================

/* Faz a amostragem de um sensor, gerando o valor da amostra.
 * Se for um sensor analógico, essa amostragem consiste em efetuar várias leituras na porta analógica,
 * conforme indicado pela constante ADCON_QUANT_LEITURAS_MEDIA_AMOSTRA e
 * efetuar a média dos valores dessas leituras, resultando no valor
 * da amostra do sensor.
 * @param num_sensor número do sensor a ser amostrado (começa em zero).
 * @return valor valor absoluto (binário) amostrado do sensor (média das leituras se for um sensor analógico). */
extern uint16_t adcon_amostra_sensor(uint8_t num_sensor);

/* Converte o valor de 10 bits do conversor AD para o valor a ser mostrado no display.
 * Essa conversão depende do tipo de sensor que está sendo utilizado. */
extern void adcon_binario_para_valor(uint16_t binario, char* p_str_valor);
    
#ifdef	__cplusplus
}
#endif
#endif	/* BASE_ADCON_H */
