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
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.
    
/*
 * Estes são os endereços da EEPROM onde ficam armazendas essas informações.
 * Algumas informações usam apenas 1 byte e outras usam 2 bytes.
 * Nos 16 primeiros bytes (0x00 a 0x0F) ficam as configurações e 
 * a partir do endereço 16 (0x10) ficarão as amostras armazenadas.
 * 
 * Para valores de 16 bits (2 bytes) está sendo utilizada a convenção Little Endian:
 * - byte menos significativo gravado na posição mais baixa de memória.
 * - byte mais significativo gravado na posição mais alta de memória.
 */ 
    
//===== Constantes Públicas ==================================================
    
#define EEPROM_END_QTDE_SENSORES_ATUAL       0x00    //1 byte: quantidade de sensores atual.
#define EEPROM_END_QTDE_SENSORES_AMOSTRADOS  0x01    //1 byte: quantidade de sensores utilizados ao gravar amostras na EEMPROM.
#define EEPROM_END_TEMPO_AQUISICAO           0x02    //2 bytes: tempo entre aquisições.
#define EEPROM_END_LEITURA_MIN               0x04    //2 bytes: valor da menor leitura armazenada.
#define EEPROM_END_LEITURA_MAX               0x06    //2 bytes: valor da maior leitura armazenada.
#define EEPROM_END_QTDE_AMOSTRAS             0x08    //1 byte: quantidade de amostras atualmente armazenadas.
//0x09
//0x0A
//0x0B
#define EEPROM_END_CHAVE_INICIALIZACAO       0x0c    //1 byte: chave de inicialização (explicação mais abaixo).
//0x0D
//0x0E
//0x0F
//Até o endereço 0x0F está reservado para parâmetros de configurações.
#define EEPROM_END_INICIO_AMOSTRAS           0x10    //1 byte: endereço inicial da EEPROM onde são armazenadas as amostras.

//A constante abaixo é o valor que será gravado na posição EEPROM_END_CHAVE_INICIALIZACAO
//para indicar que a memória EEPROM tem dados válidos.
//Se o valor contido nessa posição de memória for diferente de EEPROM_VALOR_CHAVE_INICIALIZACAO
//então será considerada que a memória EEPROM não contém dados válidos.
//Esse valor é utilizado ao se ligar o equipamento. O primeiro teste efetuado é testar se
//a chave de inicialização está correta, se estiver, então lê os parâmetros de configuração da EEPROM.
//Se a chave não estiver correta, então o módulo inicializa com as configurações padrão,
//grava elas na EEPROM e grava a chave de inicialização na EEPROM. Assim, na próxima vez que ligar o equipamento,
//a chave de inicialização está correta e será feita a leitura das configurações do equipamento.
#define EEPROM_VALOR_CHAVE_INICIALIZACAO   93  //0x5D = 01011101

    
//===== Tipos Públicos =======================================================
    
//===== Variaveis Públicas ===================================================
   
//===== Funcoes Públicas =====================================================
    
/**
 * Grava uma word (16 bits) utilizando a convenção Big Endian.
 * @param end endereco inicial da EEPROM onde será gravado o dado (word de 16 bits).
 * @param dado dado a ser gravado (word de 16 bits).
 */
extern void eeprom_grava_word(uint8_t end, uint16_t dado);

/**
 * Le uma word (16 bits) da EEPROM utilizando a convenção Big Endian.
 * @param end endereco da EEPROM onde está o primeiro byte da word de 16 bits.
 * @return valor de 16 bits lido.
 */
extern uint16_t eeprom_le_word(uint8_t end);

#ifdef	__cplusplus
}
#endif

#endif	/* BASE_EEPROM_H */

