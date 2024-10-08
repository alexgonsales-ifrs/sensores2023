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
    
//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.
    
//===== Constantes Públicas ==================================================

//Indica se está em uma sessão de monitora_grava.
//Quando a EEPROM estiver cheia, esta variável será colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta variável.
extern uint8_t serv_adcon_monitora_grava;

//===== Tipos Públicos =======================================================

//===== Variaveis Públicas ===================================================

//===== Funcoes Públicas =====================================================

/****************************************************************************
* Faz uma aquisição (lê os sensores) e armazena os valores no array
* serv_adcon_amostras[].
 ***************************************************************************/
 extern void serv_adcon_aquisicao(void);

/****************************************************************************
 * Imprime no LCD os valores que estão no array serv_adcon_amostras[].
 ***************************************************************************/
extern void serv_adcon_print(void);

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
extern uint8_t serv_adcon_aquisicao_print_grava(void);

/****************************************************************************
* Mostra no LCD os valores de uma aquisição da EEPROM. 
* O parâmetro indice_aquisicao é o índice da aquisição que será buscada da EEPROM 
* para ser mostrada no LCD.
* Se não houver aquisições, mostra mensagem adequada no LCD.
* @param indice_aquisicao índice da aquisição que será buscada da EEPROM.
*****************************************************************************/
extern void serv_adcon_print_aquisicao_da_eeprom(uint8_t indice_aquisicao);

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
extern uint8_t serv_adcon_testa_indice_aquisicao_valida(uint8_t indice);

/* Verifica se já passou a contagem de tempo para efetuar uma aquisição.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se já passou a contagem ou zero (0) se ainda não passou.
 */
extern uint8_t serv_adcon_testa_timer_tempo_aquisicao(uint16_t count);

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
extern void serv_adcon_inicializa_configuracoes_eeprom(void);

/**
 * Le as configurações da EEPROM e atualiza as variáveis globais. 
 */
extern void serv_adcon_le_configuracoes_eeprom(void);

/**
 * Limpa os seguintes dados da EEPROM:
 * - as aquisições.
 * - os valores mínimo e máximo.
 */
extern void serv_adcon_limpa_dados_eeprom(void);

/**
* Envia pela RS232 todas as amostras que estão gravadas na EEPROM e dá mensagem adequada no LCD.
* @return quantidade de bytes transmitidos.
*/
extern uint8_t serv_adcon_envia_rs232_amostras_gravadas_eeprom(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_ADCON_H */

