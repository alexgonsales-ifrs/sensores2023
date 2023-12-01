/* 
 * File:   serv_adcon.h
 * Author: alex-adm
 *
 * Created on 3 de Novembro de 2023, 14:35
 */

#ifndef SERV_ADCON_H
#define	SERV_ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include <stdint.h> //int8_t
    
//===== Constantes ===========================================================
    
//===== Tipos ================================================================

//===== Vari�veis ============================================================
    
//Indica se est� em uma sess�o de monitora_grava.
//Quando a EEPROM estiver cheia, esta vari�vel ser� colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta vari�vel.
extern uint8_t serv_adcon_monitora_grava;

//===== Fun��es ==============================================================
/**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem (l� os sensores) e imprime os valores no display.
 */
 extern void serv_adcon_amostra_print(void);
 
 /**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem (l� os sensores), imprime os valores no display e
 * grava os valores na EEPROM.
 * Atualiza a vari�vel global adcon_quant_amostras_gravadas e grava-a na EEPROM.
 * Pode atualizar as vari�veis globais adcon_leitura_min e adcon_leitura_max,
 * gravando-as tamb�m na EEPROM.
 * @return retorna a quantidade de valores que foram lidos ou 0 se 
 */
extern uint8_t serv_adcon_amostra_print_grava(void);

/*
 * Imprime no display o valor de um sensor.
 * Imprime na posi��o num_sensor o valor valor_sensor.
 * A funcao divide o valor_sensor por 10 e mostra no display o quociente e o resto, 
 * separando-os por um ponto para indicar o separador de decimal.
 * @param valor_sensor valor a ser mostrado no display.
 * @param num_sensor n�mero do sensor (0, 1, 2, 3) que est� sendo mostrado. 
 * A partir desse dado a fun��o saber� em qual posi��o do display dever� mostrar o valor_sensor.
 */
//extern void serv_adcon_print_leitura(uint16_t valor_sensor, uint8_t num_sensor);

/**
 * Mostra no LCD os valores de uma amostra da EEPROM. O par�metro indice_amostra
 * � o �ndice da amostra que ser� buscada da EEPROM para ser mostrada no LCD.
 * Se n�o houver amostras suficientes, mostra mensagem adequada no LCD.
 * @param indice_amostra �ndice da amostra que ser� buscada da EEPROM.
 */
extern void serv_adcon_print_amostra_eeprom(uint8_t indice_amostra);

/**
 * Mostra no LCD os valores Max e Min. Se n�o houver amostras armazenadas na EEPROM, 
 * mostra mensagem adequada no LCD.
 */
extern void serv_adcon_print_max_min(void);

/**
 * Verifica se um indice est� dentro da faixa de valores v�lidos para as leituras
 * que est�o armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_leituras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice a ser verificado.
 * @return retorna um (1) se o indice est� na faixa v�lida ou zero (0) se
 * n�o estiver na faixa v�lida.
 */
extern uint8_t serv_adcon_testa_indice_amostra_valido(uint8_t indice);

/* Verifica se j� passou a contagem de tempo para efetuar uma amostra.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se j� passou a contagem ou zero (0) se ainda n�o passou.
 */
extern uint8_t serv_adcon_testa_timer_tempo_amostra(uint16_t count);

/**
 * Atualiza a vari�vel adcon_cfg_quant_sensores_atual para o valor quant_sensores e
 * grava tamb�m na EEPROM.
 * @param quant_sensores valor a ser atualizado na vari�vel adcon_cfg_quant_sensores_atual.
 */
extern void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores);

/**
 * Atualiza a vari�vel adcon_cfg_tempo_amostra_atual para o valor tempo_amostra e
 * grava tamb�m na EEPROM.
 * @param tempo_amostra valor a ser atualizado na vari�vel adcon_cfg_tempo_amostra_atual.
 */
extern void serv_adcon_set_tempo_amostra_atual(uint16_t tempo_amostra);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_ADCON_H */

