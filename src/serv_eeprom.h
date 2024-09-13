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

//===== Includes =============================================================

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Constantes Públicas ==================================================

//===== Tipos Públicos =======================================================

//===== Variaveis Públicas ===================================================

//===== Funcoes Públicas =====================================================    
/**
 * Verifica se a EEPROM está inicializada, ou seja, verifica se ela contém a chave de inicialização, 
 * significando que ela contém valores válidos de configuração.
 * @return retorna 1 se a EEPROM está inicializada ou 0 se ela não está inicializada. 
 */
extern uint8_t serv_eeprom_verifica_inicializacao(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_EEPROM_H */

