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
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================

//===== Tipos P�blicos =======================================================

//===== Variaveis P�blicas ===================================================

//===== Funcoes P�blicas =====================================================    
/**
 * Verifica se a EEPROM est� inicializada, ou seja, verifica se ela cont�m a chave de inicializa��o, 
 * significando que ela cont�m valores v�lidos de configura��o.
 * @return retorna 1 se a EEPROM est� inicializada ou 0 se ela n�o est� inicializada. 
 */
extern uint8_t serv_eeprom_verifica_inicializacao(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SERV_EEPROM_H */

