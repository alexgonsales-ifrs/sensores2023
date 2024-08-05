#include <stdint.h>
#include <xc.h>
#include "base_eeprom.h"

static uint16_t eeprom_le_word_big(uint8_t end);
static uint16_t eeprom_ler_word_little(uint8_t end);
static void eeprom_grava_word_big(uint8_t end, uint16_t dado);
static void eeprom_gravar_word_little(uint8_t end, uint16_t dado);

uint16_t eeprom_le_word(uint8_t end) {
    uint16_t temp_int = 0;
    //Le o byte mais significativo (primeiro byte).
    temp_int = eeprom_read(end);
    temp_int = temp_int << 8;
    //Le o byte menos significativo (segundo byte))
    temp_int = temp_int + eeprom_read(end+1);
    return temp_int;
}

void eeprom_grava_word(uint8_t end, uint16_t dado) {
    //Grava o byte mais significativo.
    eeprom_write(end, dado >> 8);
    //Grava o byte menos significativo.
    eeprom_write(end + 1, (uint8_t)dado);
}

static uint16_t eeprom_le_word_big(uint8_t end) {
    uint16_t temp_int = 0;
    temp_int = eeprom_read(end+1);
    temp_int = temp_int + (uint16_t) (eeprom_read(end) << 8);
    return temp_int;
}

static void eeprom_grava_word_big(uint8_t end, uint16_t dado) {
    eeprom_write(end, dado >> 8);
    eeprom_write(end + 1, (uint8_t)dado);
}

static uint16_t eeprom_ler_word_little(uint8_t end) {
    /*versao little endian
    uint16_t ret;
    ret = eeprom_read(end);
    ret = ret + ( ((uint16_t)eeprom_read(end+1)) << 8);
    return ret;
    //return (eeprom_read(end) + (eeprom_read(end+1) << 8));
    */
    return 0;
}

static void eeprom_gravar_word_little(uint8_t end, uint16_t dado) {
    /*versao little endian:
    eeprom_write(end, (uint8_t) dado);
    eeprom_write(end+1, (uint8_t) (dado >> 8));
     */
}