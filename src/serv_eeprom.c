#include <xc.h>     //eeprom_read())

#include "serv_eeprom.h"
#include "base_eeprom.h"

uint8_t serv_eeprom_verifica_inicializacao(void) {
  //Testa se a EEPROM j� foi gravada pelo equipamento, retornando um boolean.
  //Se ainda n�o foi, ent�o inicializa com os valores de configura��o padr�o.
  if (eeprom_read(EEPROM_END_CHAVE_INICIALIZACAO) == EEPROM_VALOR_CHAVE_INICIALIZACAO) {
    return 1;
  }
  else {
    return 0;
  }  
}//serv_eeprom_verifica_inicializacao()
