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

//===== Variáveis ============================================================
    
//Indica se está em uma sessão de monitora_grava.
//Quando a EEPROM estiver cheia, esta variável será colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta variável.
extern uint8_t serv_adcon_monitora_grava;

//===== Funções ==============================================================
/**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem (lê os sensores) e imprime os valores no display.
 */
 extern void serv_adcon_amostra_print(void);
 
 /**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem (lê os sensores), imprime os valores no display e
 * grava os valores na EEPROM.
 * Atualiza a variável global adcon_quant_amostras_gravadas e grava-a na EEPROM.
 * Pode atualizar as variáveis globais adcon_leitura_min e adcon_leitura_max,
 * gravando-as também na EEPROM.
 * @return retorna a quantidade de valores que foram lidos ou 0 se 
 */
extern uint8_t serv_adcon_amostra_print_grava(void);

/*
 * Imprime no display o valor de um sensor.
 * Imprime na posição num_sensor o valor valor_sensor.
 * A funcao divide o valor_sensor por 10 e mostra no display o quociente e o resto, 
 * separando-os por um ponto para indicar o separador de decimal.
 * @param valor_sensor valor a ser mostrado no display.
 * @param num_sensor número do sensor (0, 1, 2, 3) que está sendo mostrado. 
 * A partir desse dado a função saberá em qual posição do display deverá mostrar o valor_sensor.
 */
//extern void serv_adcon_print_leitura(uint16_t valor_sensor, uint8_t num_sensor);

/**
 * Mostra no LCD os valores de uma amostra da EEPROM. O parâmetro indice_amostra
 * é o índice da amostra que será buscada da EEPROM para ser mostrada no LCD.
 * Se não houver amostras suficientes, mostra mensagem adequada no LCD.
 * @param indice_amostra índice da amostra que será buscada da EEPROM.
 */
extern void serv_adcon_print_amostra_eeprom(uint8_t indice_amostra);

/**
 * Mostra no LCD os valores Max e Min. Se não houver amostras armazenadas na EEPROM, 
 * mostra mensagem adequada no LCD.
 */
extern void serv_adcon_print_max_min(void);

/**
 * Verifica se um indice está dentro da faixa de valores válidos para as leituras
 * que estão armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_leituras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice a ser verificado.
 * @return retorna um (1) se o indice está na faixa válida ou zero (0) se
 * não estiver na faixa válida.
 */
extern uint8_t serv_adcon_testa_indice_amostra_valido(uint8_t indice);

/* Verifica se já passou a contagem de tempo para efetuar uma amostra.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se já passou a contagem ou zero (0) se ainda não passou.
 */
extern uint8_t serv_adcon_testa_timer_tempo_amostra(uint16_t count);

/**
 * Atualiza a variável adcon_cfg_quant_sensores_atual para o valor quant_sensores e
 * grava também na EEPROM.
 * @param quant_sensores valor a ser atualizado na variável adcon_cfg_quant_sensores_atual.
 */
extern void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores);

/**
 * Atualiza a variável adcon_cfg_tempo_amostra_atual para o valor tempo_amostra e
 * grava também na EEPROM.
 * @param tempo_amostra valor a ser atualizado na variável adcon_cfg_tempo_amostra_atual.
 */
extern void serv_adcon_set_tempo_amostra_atual(uint16_t tempo_amostra);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_ADCON_H */

