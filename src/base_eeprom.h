/* 
 * File:   base_eeprom.h
 * Author: ti
 *
 * Created on 12 de Janeiro de 2014, 04:36
 */

#ifndef BASE_EEPROM_H
#define	BASE_EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
#include <stdint.h>
    
//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.
    
/*
 * Estes s�o os endere�os da EEPROM onde ficam armazendas essas informa��es.
 * Algumas informa��es usam apenas 1 byte e outras usam 2 bytes.
 * Nos 16 primeiros bytes (0x00 a 0x0F) ficam as configura��es e 
 * a partir do endere�o 16 (0x10) ficar�o as amostras armazenadas.
 * 
 * Para valores de 16 bits (2 bytes) est� sendo utilizada a conven��o Little Endian:
 * - byte menos significativo gravado na posi��o mais baixa de mem�ria.
 * - byte mais significativo gravado na posi��o mais alta de mem�ria.
 */ 
    
//===== Constantes P�blicas ==================================================
    
#define EEPROM_END_QTDE_SENSORES_ATUAL       0x00    //1 byte: quantidade de sensores atual.
#define EEPROM_END_QTDE_SENSORES_AMOSTRADOS  0x01    //1 byte: quantidade de sensores utilizados ao gravar amostras na EEMPROM.
#define EEPROM_END_TEMPO_AQUISICAO           0x02    //2 bytes: tempo entre aquisi��es.
#define EEPROM_END_LEITURA_MIN               0x04    //2 bytes: valor da menor leitura armazenada.
#define EEPROM_END_LEITURA_MAX               0x06    //2 bytes: valor da maior leitura armazenada.
#define EEPROM_END_QTDE_AMOSTRAS             0x08    //1 byte: quantidade de amostras atualmente armazenadas.
//0x09
//0x0A
//0x0B
#define EEPROM_END_CHAVE_INICIALIZACAO       0x0c    //1 byte: chave de inicializa��o (explica��o mais abaixo).
//0x0D
//0x0E
//0x0F
//At� o endere�o 0x0F est� reservado para par�metros de configura��es.
#define EEPROM_END_INICIO_AMOSTRAS           0x10    //1 byte: endere�o inicial da EEPROM onde s�o armazenadas as amostras.

//A constante abaixo � o valor que ser� gravado na posi��o EEPROM_END_CHAVE_INICIALIZACAO
//para indicar que a mem�ria EEPROM tem dados v�lidos.
//Se o valor contido nessa posi��o de mem�ria for diferente de EEPROM_VALOR_CHAVE_INICIALIZACAO
//ent�o ser� considerada que a mem�ria EEPROM n�o cont�m dados v�lidos.
//Esse valor � utilizado ao se ligar o equipamento. O primeiro teste efetuado � testar se
//a chave de inicializa��o est� correta, se estiver, ent�o l� os par�metros de configura��o da EEPROM.
//Se a chave n�o estiver correta, ent�o o m�dulo inicializa com as configura��es padr�o,
//grava elas na EEPROM e grava a chave de inicializa��o na EEPROM. Assim, na pr�xima vez que ligar o equipamento,
//a chave de inicializa��o est� correta e ser� feita a leitura das configura��es do equipamento.
#define EEPROM_VALOR_CHAVE_INICIALIZACAO   93  //0x5D = 01011101

    
//===== Tipos P�blicos =======================================================
    
//===== Variaveis P�blicas ===================================================
   
//===== Funcoes P�blicas =====================================================
    
/**
 * Grava uma word (16 bits) utilizando a conven��o Big Endian.
 * @param end endereco inicial da EEPROM onde ser� gravado o dado (word de 16 bits).
 * @param dado dado a ser gravado (word de 16 bits).
 */
extern void eeprom_grava_word(uint8_t end, uint16_t dado);

/**
 * Le uma word (16 bits) da EEPROM utilizando a conven��o Big Endian.
 * @param end endereco da EEPROM onde est� o primeiro byte da word de 16 bits.
 * @return valor de 16 bits lido.
 */
extern uint16_t eeprom_le_word(uint8_t end);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE_EEPROM_H */

