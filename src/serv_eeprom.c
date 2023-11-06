#include <xc.h>     //eeprom_read())

#include "serv_eeprom.h"
#include "eeprom.h"
#include "adcon.h"

uint8_t serv_eeprom_verifica_inicializacao(void) {
  //Testa se a EEPROM já foi gravada pelo equipamento, retornando um boolean.
  //Se ainda não foi, então inicializa com os valores de configuração padrão.
  if (eeprom_read(EEPROM_END_CHAVE_INICIALIZACAO) == EEPROM_VALOR_CHAVE_INICIALIZACAO) {
    return 1;
  }
  else {
    return 0;
  }  
}//serv_eeprom_verifica_inicializacao()

void serv_eeprom_inicializa_configuracoes(void) {
    adcon_cfg_quant_sensores_atual      = 1;
    eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, adcon_cfg_quant_sensores_atual);
    
    //cfg_quant_sensores_amostrados = 1;
    //eeprom_grava_word(EEPROM_END_QTDE_SENSORES_AMOSTRADOS, cfg_quant_sensores_amostrados);
    
    adcon_cfg_tempo_amostra_atual       = ADCON_CFG_TEMPO_AMOSTRA_1_SEGUNDO;
    eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, adcon_cfg_tempo_amostra_atual);

    //A chave de inicializacao tem que ser a ultima a ser gravada (OBRIGATORIAMENTE),
    //pois, parece que quando o hardware gravador termina de efetuar a gravacao,
    //o microcontrolador eh ligado por um breve momento, enquanto ha energia eletrica
    //residual nos capacitores do gravador.
    //<<<<<<< isto ainda precisa ser verificado <<<<<<<<<<<<<<<<
    eeprom_write(EEPROM_END_CHAVE_INICIALIZACAO, EEPROM_VALOR_CHAVE_INICIALIZACAO);
}//serv_eeprom_inicializa_configuracoes()

void serv_eeprom_le_configuracoes(void) {
  adcon_quant_leituras_gravadas = eeprom_read(EEPROM_END_QTDE_LEITURAS);
  adcon_cfg_quant_sensores_atual      = eeprom_read(EEPROM_END_QTDE_SENSORES_ATUAL);
  adcon_cfg_tempo_amostra_atual       = eeprom_le_word(EEPROM_END_TEMPO_AMOSTRAGEM);
  adcon_leitura_min             = eeprom_le_word(EEPROM_END_LEITURA_MIN);
  adcon_leitura_max             = eeprom_le_word(EEPROM_END_LEITURA_MAX);
  //uint8_t qtd_sens     = eeprom_read(EEPROM_END_QTDE_SENSORES_AMOSTRADOS);
}//serv_eeprom_le_configuracoes()

void serv_eeprom_limpa_dados(void) {
    adcon_quant_leituras_gravadas = 0;
    eeprom_write(EEPROM_END_QTDE_LEITURAS, adcon_quant_leituras_gravadas);
    
    //Grava em leitura_min o valor MAXIMO possível.
    adcon_leitura_min = ADCON_VALOR_MAXIMO_LEITURA;
    eeprom_grava_word(EEPROM_END_LEITURA_MIN, ADCON_VALOR_MAXIMO_LEITURA);
    
    //Grava em leitura max o valor MÍNIMO possível.
    adcon_leitura_max = ADCON_VALOR_MINIMO_LEITURA;
    eeprom_grava_word(EEPROM_END_LEITURA_MAX, ADCON_VALOR_MINIMO_LEITURA);

    //Limpa toda a area das amostras.
    for (uint8_t i = 0; i < ADCON_QTD_MAX_LEITURAS * 2; i++) {
      eeprom_write(EEPROM_END_INICIO_AMOSTRAS + i, 0);
    }  
}//serv_eeprom_limpa_dados()

