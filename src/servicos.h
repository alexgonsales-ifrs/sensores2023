/* 
 * File:   servicos.h
 * Author: Alex
 *
 * Created on 2 de Novembro de 2023, 22:26
 */

#ifndef SERVICOS_H
#define	SERVICOS_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Verifica se a EEPROM está inicializada, ou seja, verifica se ela contém a chave de inicialização, 
 * significando que ela contém valores válidos de configuração.
 * @return retorna 1 se a EEPROM está inicializada ou 0 se ela não está inicializada. 
 */
extern uint8_t serv_eeprom_verifica_inicializacao(void);

/**
 * Inicializa as configurações da EPRROM com os valores dafault, 
 * gravando também a chave de inicialização.
 */
extern void serv_eeprom_inicializa_configuracoes(void);

/**
 * Le as configurações da EEPROM e atualiza as variáveis globais. 
 */
extern void serv_eeprom_le_configuracoes(void);

/**
 * Limpaa os dados de leitura e os valores mínimo e máximo da EEPROM.
 */
extern void serv_eeprom_limpa_dados(void);

/**
* Envia pela RS232 todas as leituras que estão gravadas na EEPROM.
* @return quantidade de bytes transmitidos
*/
extern uint8_t serv_rs232_envia_leituras_gravadas_eeprom(void);

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
 */
extern void serv_adcon_amostra_print_grava(void);

/*
 * Imprime no display o valor de um sensor.
 * Imprime na posição num_sensor o valor valor_sensor.
 * A funcao divide o valor_sensor por 10 e mostra no display o quociente e o resto, 
 * separando-os por um ponto para indicar o separador de decimal.
 * @param valor_sensor valor a ser mostrado no display.
 * @param num_sensor número do sensor (0, 1, 2, 3) que está sendo mostrado. 
 * A partir desse dado a função saberá em qual posição do display deverá mostrar o valor_sensor.
 */
extern void serv_adcon_print(uint16_t valor_sensor, uint8_t num_sensor);

#ifdef	__cplusplus
}
#endif

#endif	/* SERVICOS_H */

