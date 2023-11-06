/*****************************************************************************
 * File:   adcon.h
 * Author: alexdg
 * Comments:
 *
 * Definições importantes para estes contexto:
 * 
 * Aquisicao: é a aquisição de um valor da porta analógica.
 *  
 * Leitura: é a aquisição de vários valores da porta analógica, resultando no valor médio de leitura e que será considerado 
 * como o valor lido do sensor. Por exemplo, uma leitura de um sensor pode corresponder à aquisição de 10 valores.
 * 
 * Amostra: é um conjunto de leitura de todos os sensores atualmente configurados.
 * Por exemplo, se está configurado para 4 sensores, então uma amostra corresponde a 4 leituras.
 * Supondo que cada leitura corresponda à média da aquisição de 10 valores,
 * então uma amostra corresponderia à aquisição de 40 valores (armazenamento de apenas 4 dados de leitura).
 * 
 * Sessão de monitoramento: corresponde a uma sessão onde são realizadas várias amostras.
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

  //Quantidade máxima de leituras permitidas (gravadas na EEPROM). Cada leitura ocupa 2 bytes.
  //A quantidade de amostras dependerá da quantidade de sensores que está configurado.
  //Quantidade máxima de amostras = QUANT_MAX_LEITURAS / QUANT_SENSORES
  #define ADCON_QTD_MAX_LEITURAS              120 
    
  //As constantes abaixo são utilizadas para inicialização das variáveis adcon_leitura_min e adcon_leitura_max.
  //São os valores mínimo e máximo possíveis na leitura de um sensor.
  //Apesar do conversor analógico/digital ter resolução de 10 bits, o que daria o valor máximo de 1023,
  //estamos utilizando como valor máximo 65535 = 0xFFFF (máximo valor possível em 16 bits).
  #define ADCON_VALOR_MINIMO_LEITURA          0
  #define ADCON_VALOR_MAXIMO_LEITURA          0xFFFF

  //Quantidade máxima possível de sensores.
  #define ADCON_CFG_QUANT_SENSORES_MAX 4
  
  //É a quantidade de sensores que estava configurada a última vez que
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

//===== Variáveis ============================================================

//Matém o valor da mínima leitura ocorrida. É inicializada com ADCON_VALOR_MAXIMO_LEITURA.
extern uint16_t adcon_leitura_min;

//Matém o valor da maior leitura ocorrida. É inicializada com ADCON_VALOR_MINIMO_LEITURA.
extern uint16_t adcon_leitura_max;

//Mantém a quantidade de leituras que foram gravadas na EEPROM.
extern uint8_t  adcon_quant_leituras_gravadas;

//Temo entre amostras atualmente configurado.
extern uint16_t adcon_cfg_tempo_amostra_atual;

//Quantidade de sensores atualmente configurada.
extern uint8_t adcon_cfg_quant_sensores_atual;

//===== Funções ==============================================================

/**
 * Funcao que inicializa o conversor analogico/digital.
 * É chamada na funcao main().
*/
extern void adcon_init(void);

/*
 * Funcao que faz a leitura de um sensor.
 * Essa leitura consiste em efetuar várias aquisições na porta analógica,
 * conforme indicado pela constante ADCON_QUANT_AQUISICOES_MEDIA_LEITURA e
 * efetuar a média dos valores dessas aquisições, resultando no valor
 * da leitura do sensor.
 * @param num_sensor número do sensor a ser lido (0 a 3).
 * @return valor varlor lido do sensor (média das aquisições).
 */
extern uint16_t adcon_leitura_sensor(uint8_t num_sensor);

#ifdef	__cplusplus
}
#endif
#endif	/* ADCON_H */
