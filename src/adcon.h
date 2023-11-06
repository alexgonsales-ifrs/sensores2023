/*****************************************************************************
 * File:   adcon.h
 * Author: alexdg
 * Comments:
 *
 * Defini��es importantes para estes contexto:
 * 
 * Aquisicao: � a aquisi��o de um valor da porta anal�gica.
 *  
 * Leitura: � a aquisi��o de v�rios valores da porta anal�gica, resultando no valor m�dio de leitura e que ser� considerado 
 * como o valor lido do sensor. Por exemplo, uma leitura de um sensor pode corresponder � aquisi��o de 10 valores.
 * 
 * Amostra: � um conjunto de leitura de todos os sensores atualmente configurados.
 * Por exemplo, se est� configurado para 4 sensores, ent�o uma amostra corresponde a 4 leituras.
 * Supondo que cada leitura corresponda � m�dia da aquisi��o de 10 valores,
 * ent�o uma amostra corresponderia � aquisi��o de 40 valores (armazenamento de apenas 4 dados de leitura).
 * 
 * Sess�o de monitoramento: corresponde a uma sess�o onde s�o realizadas v�rias amostras.
 * 
 * Revision history: 
 *
 ****************************************************************************/

#ifndef ADCON_H
#define	ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
    
//===== Constantes ===========================================================

  //Quantidade m�xima de leituras permitidas (gravadas na EEPROM). Cada leitura ocupa 2 bytes.
  //A quantidade de amostras depender� da quantidade de sensores que est� configurado.
  //Quantidade m�xima de amostras = QUANT_MAX_LEITURAS / QUANT_SENSORES
  #define ADCON_QTD_MAX_LEITURAS              120 
    
  //As constantes abaixo s�o utilizadas para inicializa��o das vari�veis adcon_leitura_min e adcon_leitura_max.
  //S�o os valores m�nimo e m�ximo poss�veis na leitura de um sensor.
  //Apesar do conversor anal�gico/digital ter resolu��o de 10 bits, o que daria o valor m�ximo de 1023,
  //estamos utilizando como valor m�ximo 65535 = 0xFFFF (m�ximo valor poss�vel em 16 bits).
  #define ADCON_VALOR_MINIMO_LEITURA          0
  #define ADCON_VALOR_MAXIMO_LEITURA          0xFFFF

  //Quantidade m�xima poss�vel de sensores.
  #define ADCON_CFG_QUANT_SENSORES_MAX 4
  
  //� a quantidade de sensores que estava configurada a �ltima vez que
  //foi feito um monitoramento.
  //extern uint8_t cfg_quant_sensores_amostrados;

  // Valores de selecao do tempo de amostragem (tempo entre amostras)
  #define     ADCON_CFG_TEMPO_AMOSTRA_1_SEGUNDO     18
  #define     ADCON_CFG_TEMPO_AMOSTRA_10_SEGUNDOS   180
  #define     ADCON_CFG_TEMPO_AMOSTRA_30_SEGUNDOS   540
  #define     ADCON_CFG_TEMPO_AMOSTRA_1_MINUTO      1080
  #define     ADCON_CFG_TEMPO_AMOSTRA_10_MINUTOS    10800
  #define     ADCON_CFG_TEMPO_AMOSTRA_30_MINUTOS    32400
  #define     ADCON_CFG_TEMPO_AMOSTRA_1_HORA        64800

//===== Tipos ================================================================

//===== Vari�veis ============================================================

//Mat�m o valor da m�nima leitura ocorrida. � inicializada com ADCON_VALOR_MAXIMO_LEITURA.
extern uint16_t adcon_leitura_min;

//Mat�m o valor da maior leitura ocorrida. � inicializada com ADCON_VALOR_MINIMO_LEITURA.
extern uint16_t adcon_leitura_max;

//Mant�m a quantidade de leituras que foram gravadas na EEPROM.
extern uint8_t  adcon_quant_leituras_gravadas;

//Temo entre amostras atualmente configurado.
extern uint16_t adcon_cfg_tempo_amostra_atual;

//Quantidade de sensores atualmente configurada.
extern uint8_t adcon_cfg_quant_sensores_atual;

//===== Fun��es ==============================================================

/**
 * Funcao que inicializa o conversor analogico/digital.
 * � chamada na funcao main().
*/
extern void adcon_init(void);

/*
 * Funcao que faz a leitura de um sensor.
 * Essa leitura consiste em efetuar v�rias aquisi��es na porta anal�gica,
 * conforme indicado pela constante ADCON_QUANT_AQUISICOES_MEDIA_LEITURA e
 * efetuar a m�dia dos valores dessas aquisi��es, resultando no valor
 * da leitura do sensor.
 * @param num_sensor n�mero do sensor a ser lido (0 a 3).
 * @return valor varlor lido do sensor (m�dia das aquisi��es).
 */
extern uint16_t adcon_leitura_sensor(uint8_t num_sensor);

#ifdef	__cplusplus
}
#endif
#endif	/* ADCON_H */
