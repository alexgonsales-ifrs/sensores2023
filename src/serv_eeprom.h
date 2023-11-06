/* 
 * File:   serv_eeprom.h
 * Author: Alex
 *
 * Created on 2 de Novembro de 2023, 22:26
 */

#ifndef SERV_EEPROM_H
#define	SERV_EEPROM_H

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

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_EEPROM_H */

