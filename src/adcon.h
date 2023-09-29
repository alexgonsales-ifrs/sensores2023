/* 
 * File:   adcon.h
 *
 * Defini��es importantes para estes contexto:
 * 
 * Aquisicao: � a aquisi��o de um valor da porta anal�gica.
 *  
 * Leitura: � a aquisi��o de v�rios valores da porta anal�gica, resultando no valor m�dio de leitura e que ser� considerado 
 * como o valor lido do sensor. Por exemplo, uma leitura de um sensor pode corresponder a aquisi��o de 10 valores.
 * 
 * Amostra: � um conjunto de leitura de todos os sensores atualmente configurados.
 * Por exemplo, se est� configurado para 4 sensores, ent�o uma amostra corresponde a 4 leituras.
 * Nesse caso, se cada leitura corresponde � m�dia da aquisi��o de 10 valores,
 * ent�o uma amostra corresponderia a aquisi��o de 40 valores (armazenamento de apenas 4 dados de leitura).
 * 
 * Sess�o de monitoramento: corresponde a uma sess�o onde s�o realizadas (lidas) v�rias amostras.
 * 
 */

#ifndef ADCON_H
#define	ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//Quantidade m�xima de leituras permitidas (gravadas na EEPROM). Cada leitura ocupa 2 bytes.
//A quantidade de amostras depender� da quantidade de sensores que est� configurado.
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
