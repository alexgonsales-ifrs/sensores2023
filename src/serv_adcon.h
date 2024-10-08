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
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.
    
//===== Constantes P�blicas ==================================================

//Indica se est� em uma sess�o de monitora_grava.
//Quando a EEPROM estiver cheia, esta vari�vel ser� colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta vari�vel.
extern uint8_t serv_adcon_monitora_grava;

//===== Tipos P�blicos =======================================================

//===== Variaveis P�blicas ===================================================

//===== Funcoes P�blicas =====================================================

/****************************************************************************
* Faz uma aquisi��o (l� os sensores) e armazena os valores no array
* serv_adcon_amostras[].
 ***************************************************************************/
 extern void serv_adcon_aquisicao(void);

/****************************************************************************
 * Imprime no LCD os valores que est�o no array serv_adcon_amostras[].
 ***************************************************************************/
extern void serv_adcon_print(void);

 /****************************************************************************
* Faz uma aquisi��o (l� os sensores), imprime os valores no display e
* grava os valores na EEPROM.
* Atualiza a vari�vel global adcon_quant_amostras_gravadas e grava-a na EEPROM.
* Pode atualizar as vari�veis globais adcon_amostra_min e adcon_amostra_max,
* gravando-as tamb�m na EEPROM.
* @return Retorna a quantidade de amostras que foram lidas ou 0 caso n�o tenha espa�o na EEPROM para gravar. 
* Se n�o houver espa�o na EEPROM, a fun��o sinaliza colocando zero na vari�vel global serv_adcon_monitora_grava.
* Quem chamou esta fun��o ser� respons�vel por interromper o monitoramento.
*****************************************************************************/
extern uint8_t serv_adcon_aquisicao_print_grava(void);

/****************************************************************************
* Mostra no LCD os valores de uma aquisi��o da EEPROM. 
* O par�metro indice_aquisicao � o �ndice da aquisi��o que ser� buscada da EEPROM 
* para ser mostrada no LCD.
* Se n�o houver aquisi��es, mostra mensagem adequada no LCD.
* @param indice_aquisicao �ndice da aquisi��o que ser� buscada da EEPROM.
*****************************************************************************/
extern void serv_adcon_print_aquisicao_da_eeprom(uint8_t indice_aquisicao);

/**
 * Mostra no LCD os valores Max e Min. Se n�o houver amostras armazenadas na EEPROM, 
 * mostra mensagem adequada no LCD.
 */
extern void serv_adcon_print_max_min(void);

/**
 * Verifica se um indice est� dentro da faixa de valores v�lidos para as aquisi��es
 * que est�o armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_amostras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice da aquisi��o a ser verificada.
 * @return retorna um (1) se o indice est� na faixa v�lida ou zero (0) se
 * n�o estiver na faixa v�lida.
 */
extern uint8_t serv_adcon_testa_indice_aquisicao_valida(uint8_t indice);

/* Verifica se j� passou a contagem de tempo para efetuar uma aquisi��o.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se j� passou a contagem ou zero (0) se ainda n�o passou.
 */
extern uint8_t serv_adcon_testa_timer_tempo_aquisicao(uint16_t count);

/**
 * Atualiza a vari�vel adcon_cfg_quant_sensores_atual para o valor quant_sensores e
 * grava tamb�m na EEPROM.
 * @param quant_sensores valor a ser atualizado na vari�vel adcon_cfg_quant_sensores_atual.
 */
extern void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores);

/**
 * Configura o tempo entre aquisi��es. 
 * Atualiza a vari�vel adcon_cfg_tempo_aquisicao_atual para o valor tempo_aquisicao e
 * grava tamb�m na EEPROM.
 * @param tempo_aquisicao novo valor para o tempo entre aquisi��es.
 */
extern void serv_adcon_set_tempo_aquisicao_atual(uint16_t tempo_aquisicao);

/**
 * Inicializa as configura��es da EPRROM com os valores dafault, 
 * gravando tamb�m a chave de inicializa��o.
 */
extern void serv_adcon_inicializa_configuracoes_eeprom(void);

/**
 * Le as configura��es da EEPROM e atualiza as vari�veis globais. 
 */
extern void serv_adcon_le_configuracoes_eeprom(void);

/**
 * Limpa os seguintes dados da EEPROM:
 * - as aquisi��es.
 * - os valores m�nimo e m�ximo.
 */
extern void serv_adcon_limpa_dados_eeprom(void);

/**
* Envia pela RS232 todas as amostras que est�o gravadas na EEPROM e d� mensagem adequada no LCD.
* @return quantidade de bytes transmitidos.
*/
extern uint8_t serv_adcon_envia_rs232_amostras_gravadas_eeprom(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_ADCON_H */

