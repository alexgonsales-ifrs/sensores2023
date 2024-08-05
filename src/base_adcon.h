/*****************************************************************************
 * File:   base_adcon.h
 * Author: alexdg
 * Comments:
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
 * Revision history: 
 *
 ****************************************************************************/

#ifndef BASE_ADCON_H
#define	BASE_ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
    
//===== Constantes ===========================================================

  //Quantidade máxima de amostras permitidas (gravadas na EEPROM). Cada amostra ocupa 2 bytes.
  //A quantidade de aquisições dependerá da quantidade de sensores que está configurado.
  //Quantidade máxima de aquisições = QUANT_MAX_AMOSTRAS / QUANT_SENSORES
  #define ADCON_QTD_MAX_AMOSTRAS              120 
    
  //As constantes abaixo são utilizadas para inicialização das variáveis adcon_amostra_min e adcon_amostra_max.
  //São os valores mínimo e máximo possíveis na amostra de um sensor.
  //Apesar do conversor analógico/digital ter resolução de 10 bits, o que daria o valor máximo de 1023,
  //estamos utilizando como valor máximo 65535 = 0xFFFF (máximo valor possível em 16 bits).
  #define ADCON_VALOR_MINIMO_AMOSTRA          0
  #define ADCON_VALOR_MAXIMO_AMOSTRA          0xFFFF

  //Quantidade máxima possível de sensores.
  #define ADCON_CFG_QUANT_SENSORES_MAX 4
  
  //É a quantidade de sensores que estava configurada a última vez que
  //foi feito um monitoramento.
  //extern uint8_t cfg_quant_sensores_amostrados;

  // Valores de selecao do tempo entre aquisições:
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO     18
  #define     ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS   180
  #define     ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS   540
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO      1080
  #define     ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS    10800
  #define     ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS    32400
  #define     ADCON_CFG_TEMPO_AQUISICAO_1_HORA        64800

//===== Tipos ================================================================

//===== Variáveis ============================================================

//Matém o valor da mínima amostra ocorrida. É inicializada com ADCON_VALOR_MAXIMO_AMOSTRA.
extern uint16_t adcon_amostra_min;

//Matém o valor da maior leitura ocorrida. É inicializada com ADCON_VALOR_MINIMO_AMOSTRA.
extern uint16_t adcon_amostra_max;

//Mantém a quantidade de amostras que foram gravadas na EEPROM.
extern uint8_t  adcon_quant_amostras_gravadas;

//Temo entre aquisições atualmente configurado.
extern uint16_t adcon_cfg_tempo_aquisicao_atual;

//Quantidade de sensores atualmente configurada.
extern uint8_t adcon_cfg_quant_sensores_atual;

//===== Funções ==============================================================

/**
 * Funcao que inicializa o conversor analogico/digital.
 * É chamada na funcao main().
*/
extern void adcon_init(void);

/*
 * Funcao que faz a amostragem de um sensor, gerando o valor da amostra.
 * Essa amostragem consiste em efetuar várias leituras na porta analógica,
 * conforme indicado pela constante ADCON_QUANT_LEITURAS_PARA_MEDIA_AMOSTRA e
 * efetuar a média dos valores dessas leituras, resultando no valor
 * da amostra do sensor.
 * @param num_sensor número do sensor a ser amostrado (0 a 3).
 * @return valor valor amostrado do sensor (média das leituras).
 */
extern uint16_t adcon_amostra_sensor(uint8_t num_sensor);

#ifdef	__cplusplus
}
#endif
#endif	/* BASE_ADCON_H */
