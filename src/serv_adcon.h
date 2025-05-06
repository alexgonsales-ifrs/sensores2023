/* 
 * File:   serv_adcon.h
 * Author: alex
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
    
//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.
    
//===== Constantes Públicas ==================================================

//Indica se está em uma sessão MONITORA_GRAVA.
//Quando a EEPROM estiver cheia, esta variável será colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta variável
//e interromper a sessão de monitoramento e gravação.
extern uint8_t serv_adcon_bol_monitora_grava;

//===== Tipos Públicos =======================================================

//===== Variaveis Públicas ===================================================

/*
Indica a quantidade de sensores a serem mostrados em uma linha do LCD.
Valor deve ser 1, 2 ou 4.

1 sensor por linha: Mostra label do sensor(8 caracteres), valor real com 1 casa decimal (5 caracteres), unidade (3 caracteres).
|----------------|
|s1Butano90000ppm|
|s2Temp  102.3ºC |
|----------------|

|----------------|
|s1Temp  102.3ºC |
|s2Temp   40.8ºF |
|----------------|

2 sensores por linha: mostra número do sensor (0 a 9) e valor real da amostra com 1 casa decimal (5 caracteres).
|----------------|
|1=100.5| 2= 90.8|
|3=  5.9| 4=100.7|
|----------------|

2 sensores por linha: mostra label do sensor (3 caracteres) e valor absoluto (4 digitos).
|----------------|
|tp1=1023 t2=  99|
|but=  91pro=1023|
|----------------|

4 sensores por linha: mostra somente o valor absoluto da amostra (0 a 1023).
|----------------|
|10231000   3 100| valores: 1023 1000    3 100
|   910051023 300| valores:    9 1005 1023 300
|----------------|
 
 Poderia ter um multiplicador ou divisor (100, por exemplo).
 Ao imprimir, se o multiplicador extrapolar, indicar no display.
|----------------|
|1023   5  28 500| 102.300ppm     500ppm   2.800ppm 50.000ppm
|   910051023 300|  90.000ppm 100.500ppm 102.300ppm 30.000ppm
|----------------|
 
 

 Ao navegar pelas aquisições gravadas, poderia mostrar na primeira linha
 o número da aquisição sendo mostrada e o número da amostra (sensor):
|----------------|
|Aq 035/1000 s1/8| Aquisição 35 de 1000 e sensor 1 e 2 de 8.
|2Temp    40.8ºF |
|----------------|

|----------------|
|Aq 035/1000 s1/8| Aquisição 35 de 1000 e sensores 1,2,3 e 4 de 8.
|   910051023 300| valores:    9 1005 1023 300
|----------------|

|a1234 b1234c1234|   910051023 300| valores:    9 1005 1023 300
 
* 
 * 
 * */
extern uint8_t serv_adcon_quant_sensores_por_linha_lcd;

//===== Funcoes Públicas =====================================================

/****************************************************************************
* Faz uma aquisição (lê os sensores) e armazena os valores no array
* serv_adcon_amostras[].
 ***************************************************************************/
 extern void serv_adcon_aquisicao_amostrar(void);

/****************************************************************************
 * Imprime no LCD os valores que estão no array serv_adcon_amostras[].
 * @param quant_sensores quantidade de sensores a imprimir.
 ***************************************************************************/
extern void serv_adcon_aquisicao_imprimir(uint8_t quant_sens);

 /****************************************************************************
* Faz uma aquisição (lê os sensores), imprime os valores no display e
* grava os valores na EEPROM.
* Atualiza a variável global adcon_quant_amostras_gravadas e grava-a na EEPROM.
* Pode atualizar as variáveis globais adcon_amostra_min e adcon_amostra_max,
* gravando-as também na EEPROM.
* @return Retorna a quantidade de amostras que foram lidas ou 0 caso não tenha espaço na EEPROM para gravar. 
* Se não houver espaço na EEPROM, a função sinaliza colocando zero na variável global serv_adcon_monitora_grava.
* Quem chamou esta função será responsável por interromper o monitoramento.
*****************************************************************************/
extern uint8_t serv_adcon_aquisicao_amostrar_imprimir_gravar(void);

/****************************************************************************
* Mostra no LCD os valores de uma aquisição da EEPROM. 
* O parâmetro indice_aquisicao é o índice da aquisição que será buscada da EEPROM 
* para ser mostrada no LCD.
* Se não houver aquisições, mostra mensagem adequada no LCD.
* @param indice_aquisicao índice da aquisição que será buscada da EEPROM.
*****************************************************************************/
extern void serv_adcon_aquisicao_imprimir_da_eeprom(uint8_t indice_aquisicao);

/**
 * Mostra no LCD os valores Max e Min. Se não houver amostras armazenadas na EEPROM, 
 * mostra mensagem adequada no LCD.
 */
extern void serv_adcon_print_max_min(void);

/**
 * Verifica se um indice está dentro da faixa de valores válidos para as aquisições
 * que estão armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_amostras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice da aquisição a ser verificada.
 * @return retorna um (1) se o indice está na faixa válida ou zero (0) se
 * não estiver na faixa válida.
 */
extern uint8_t serv_adcon_aquisicao_testa_indice_valido(uint8_t indice);

/* Verifica se já passou a contagem de tempo para efetuar uma aquisição.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se já passou a contagem ou zero (0) se ainda não passou.
 */
extern uint8_t serv_adcon_testa_contagem_timer(uint16_t count);

/**
 * Atualiza a variável adcon_cfg_quant_sensores_atual para o valor quant_sensores e
 * grava também na EEPROM.
 * @param quant_sensores valor a ser atualizado na variável adcon_cfg_quant_sensores_atual.
 */
extern void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores);

/**
 * Configura o tempo entre aquisições. 
 * Atualiza a variável adcon_cfg_tempo_aquisicao_atual para o valor tempo_aquisicao e
 * grava também na EEPROM.
 * @param tempo_aquisicao novo valor para o tempo entre aquisições.
 */
extern void serv_adcon_set_tempo_aquisicao_atual(uint16_t tempo_aquisicao);

/**
 * Inicializa as configurações da EPRROM com os valores dafault, 
 * gravando também a chave de inicialização.
 */
extern void serv_adcon_eeprom_inicializa_configuracoes(void);

/**
 * Le as configurações da EEPROM e atualiza as variáveis globais. 
 */
extern void serv_adcon_eeprom_le_configuracoes(void);

/**
 * Limpa os seguintes dados da EEPROM:
 * - as aquisições.
 * - os valores mínimo e máximo.
 */
extern void serv_adcon_eeprom_limpa_dados(void);

/**
* Envia pela RS232 todas as amostras que estão gravadas na EEPROM e dá mensagem adequada no LCD.
* @return quantidade de bytes transmitidos.
*/
extern uint8_t serv_adcon_rs232_envia_amostras_gravadas_eeprom(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_ADCON_H */

