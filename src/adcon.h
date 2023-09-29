/* 
 * File:   adcon.h
 *
 * Definições importantes para estes contexto:
 * 
 * Aquisicao: é a aquisição de um valor da porta analógica.
 *  
 * Leitura: é a aquisição de vários valores da porta analógica, resultando no valor médio de leitura e que será considerado 
 * como o valor lido do sensor. Por exemplo, uma leitura de um sensor pode corresponder a aquisição de 10 valores.
 * 
 * Amostra: é um conjunto de leitura de todos os sensores atualmente configurados.
 * Por exemplo, se está configurado para 4 sensores, então uma amostra corresponde a 4 leituras.
 * Nesse caso, se cada leitura corresponde à média da aquisição de 10 valores,
 * então uma amostra corresponderia a aquisição de 40 valores (armazenamento de apenas 4 dados de leitura).
 * 
 * Sessão de monitoramento: corresponde a uma sessão onde são realizadas (lidas) várias amostras.
 * 
 */

#ifndef ADCON_H
#define	ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//Quantidade máxima de leituras permitidas (gravadas na EEPROM). Cada leitura ocupa 2 bytes.
//A quantidade de amostras dependerá da quantidade de sensores que está configurado.
//QUANT_MAX_AMOSTRAS = QUANT_MAX_LEITURAS / QUANT_SENSORES
#define ADCON_QTD_MAX_LEITURAS              120 
    
#define ADCON_VALOR_MAXIMO_LEITURA          0xFFFF
#define ADCON_VALOR_MINIMO_LEITURA          0
    
extern uint8_t adcon_quant_amostras_gravadas;

extern void adcon_init(void);

extern void adcon_amostra_print(void);

extern void adcon_amostra_print_grava(void);

extern void adcon_print(uint16_t valor_sensor, uint8_t num_sensor);

#ifdef	__cplusplus
}
#endif

#endif	/* ADCON_H */
