/*****************************************************************************
 * File:   base_adcon.h
 * Author: alexdg
 * Comments:
 *
 * Defini��es importantes para este contexto:
 * 
 * Leitura: � a leitura de um valor da porta anal�gica.
 *  
 * Amostra: � a leitura de v�rios valores da porta anal�gica, resultando no valor m�dio, sendo este considerado 
 * como o valor lido (amostrado) do sensor. Por exemplo, uma amostra de um sensor pode corresponder � leitura de 10 valores do sensor.
 * 
 * Aquisi��o: � um conjunto de amostras de todos os sensores atualmente configurados.
 * Por exemplo, se est� configurado para 4 sensores, ent�o uma aquisi��o corresponde a 4 amostras.
 * Supondo que cada amostra corresponda � m�dia da leitura de 10 valores,
 * ent�o uma aquisi��o corresponder� � leitura de 40 valores (armazenamento de apenas 4 amostras).
 * 
 * Sess�o de monitoramento: corresponde a uma sess�o onde s�o realizadas v�rias aquisi��es, 
 * todas com o mesmo intervalo de tempo (tempo entre aquis��es), conforme configura��o escolhida pelo usu�rio.
 * 
 * Revision history: 
 *
 ****************************************************************************/

//teste alex commmit

#ifndef BASE_ADCON_H
#define	BASE_ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include "versao.h"
    
//===== Constantes ===========================================================

  //As constantes abaixo s�o utilizadas para inicializa��o das vari�veis adcon_amostra_min e adcon_amostra_max.
  //S�o os valores m�nimo e m�ximo poss�veis na amostra de um sensor.
  //Apesar do conversor anal�gico/digital ter resolu��o de 10 bits, o que daria o valor m�ximo de 1023,
  //estamos utilizando como valor m�ximo 65535 = 0xFFFF (m�ximo valor poss�vel em 16 bits).
  #define ADCON_VALOR_MINIMO_AMOSTRA          0
  #define ADCON_VALOR_MAXIMO_AMOSTRA          0xFFFF

  //Quantidade m�xima poss�vel de sensores.
  #if defined(_HARDWARE_2013_)
    #define ADCON_CFG_QUANT_MAX_SENSORES_ANALOGICOS 4
  #elif defined(_HARDWARE_2016_)
    //8 sensores MQ.
    #define ADCON_CFG_QUANT_MAX_SENSORES_ANALOGICOS 8
  #endif

  //verificar<<<< 
  //1 sensor digital DHT22 que contar� como 2 sensores (temperatura e umidade).
    
  //Quantidade m�xima de amostras permitidas (gravadas na EEPROM). 
  //A EEPROM tem 240 bytes dispon�veis para isso.
  //Cada amostra ocupa 2 bytes, portanto s�o poss�veis 120 amostras.
  //A quantidade de aquisi��es depender� da quantidade de sensores que est� configurado.
  //Quantidade m�xima de aquisi��es = QUANT_MAX_AMOSTRAS / QUANT_SENSORES
  #define ADCON_QTD_MAX_AMOSTRAS              120 

  //� a quantidade de sensores que estava configurada a �ltima vez que
  //foi feito um monitoramento.
  //extern uint8_t cfg_quant_sensores_amostrados;

  // Valores de selecao do tempo entre aquisi��es:
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO     18
  #define     ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS   180
  #define     ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS   540
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO      1080
  #define     ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS    10800
  #define     ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS    32400
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_HORA        64800

//===== Tipos ================================================================

//===== Vari�veis ============================================================

//Mat�m o valor da m�nima amostra ocorrida. � inicializada com ADCON_VALOR_MAXIMO_AMOSTRA.
extern uint16_t adcon_amostra_min;

//Mat�m o valor da maior leitura ocorrida. � inicializada com ADCON_VALOR_MINIMO_AMOSTRA.
extern uint16_t adcon_amostra_max;

//Mant�m a quantidade de amostras que foram gravadas na EEPROM.
extern uint8_t  adcon_quant_amostras_gravadas;

//Temo entre aquisi��es atualmente configurado.
extern uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
extern uint8_t adcon_cfg_quant_sensores_atual;

//===== Fun��es ==============================================================

/*
 * Funcao que faz a amostragem de um sensor, gerando o valor da amostra.
 * Essa amostragem consiste em efetuar v�rias leituras na porta anal�gica,
 * conforme indicado pela constante ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA e
 * efetuar a m�dia dos valores dessas leituras, resultando no valor
 * da amostra do sensor.
 * @param num_sensor n�mero do sensor a ser amostrado (0 a 3).
 * @return valor valor amostrado do sensor (m�dia das leituras).
 */
extern uint16_t adcon_amostra_sensor(uint8_t num_sensor);

extern void adcon_binario_para_valor(uint16_t binario, char* p_str_valor);
    
#ifdef	__cplusplus
}
#endif
#endif	/* BASE_ADCON_H */
