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
 * Verifica se a EEPROM est� inicializada, ou seja, verifica se ela cont�m a chave de inicializa��o, 
 * significando que ela cont�m valores v�lidos de configura��o.
 * @return retorna 1 se a EEPROM est� inicializada ou 0 se ela n�o est� inicializada. 
 */
extern uint8_t serv_eeprom_verifica_inicializacao(void);

/**
 * Inicializa as configura��es da EPRROM com os valores dafault, 
 * gravando tamb�m a chave de inicializa��o.
 */
extern void serv_eeprom_inicializa_configuracoes(void);

/**
 * Le as configura��es da EEPROM e atualiza as vari�veis globais. 
 */
extern void serv_eeprom_le_configuracoes(void);

/**
 * Limpaa os dados de leitura e os valores m�nimo e m�ximo da EEPROM.
 */
extern void serv_eeprom_limpa_dados(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_EEPROM_H */

