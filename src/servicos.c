#include <xc.h>     //eeprom_read())
#include <stdlib.h> //div_t
#include <stdio.h>  //sprintf()

#include "servicos.h"
#include "cfg_quant_sensores.h"
#include "cfg_tempo_amostra.h"
#include "eeprom.h"
#include "adcon.h"
#include "rs232.h"
#include "lcd.h"

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
    cfg_quant_sensores_atual      = 1;
    eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, cfg_quant_sensores_atual);
    
    //cfg_quant_sensores_amostrados = 1;
    //eeprom_grava_word(EEPROM_END_QTDE_SENSORES_AMOSTRADOS, cfg_quant_sensores_amostrados);
    
    cfg_tempo_amostra_atual       = CFG_TEMPO_AMOSTRA_1_SEGUNDO;
    eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, cfg_tempo_amostra_atual);

    //A chave de inicializacao tem que ser a ultima a ser gravada (OBRIGATORIAMENTE),
    //pois, parece que quando o hardware gravador termina de efetuar a gravacao,
    //o microcontrolador eh ligado por um breve momento, enquanto ha energia eletrica
    //residual nos capacitores do gravador.
    //<<<<<<< isto ainda precisa ser verificado <<<<<<<<<<<<<<<<
    eeprom_write(EEPROM_END_CHAVE_INICIALIZACAO, EEPROM_VALOR_CHAVE_INICIALIZACAO);
}//serv_eeprom_inicializa_configuracoes()

void serv_eeprom_le_configuracoes(void) {
  adcon_quant_leituras_gravadas = eeprom_read(EEPROM_END_QTDE_LEITURAS);
  cfg_quant_sensores_atual      = eeprom_read(EEPROM_END_QTDE_SENSORES_ATUAL);
  cfg_tempo_amostra_atual       = eeprom_le_word(EEPROM_END_TEMPO_AMOSTRAGEM);
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

/**
 * Envia pela RS232 todas as leituras que estão gravadas na EEPROM.
 * @return 
 */
uint8_t serv_rs232_envia_leituras_gravadas_eeprom(void) {
  uint8_t dado;

  //2400 bauds = (0,000416667 s) * (10 bits em cada transmissao serial) = 4.16 ms
    
  //O java trabalha com inteiros de 16 bits com sinal (little endian) então
  //para facilitar todos os dados serão enviados como words de 16 bits.
    
  //Envia a informação da quantidade de sensores atualmente configurados (Big Endian).
  rs232_envia_byte(0);
  rs232_envia_byte(cfg_quant_sensores_atual);  
  //Envia pela RS232 as leituras que estão armazenadas na EEPROM.    
  uint8_t i;
  for (i=0; i < adcon_quant_leituras_gravadas; i = i + 2) {
      //Envia o byte mais significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
      rs232_envia_byte(dado);
      //Envia o byte menos significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
      rs232_envia_byte(dado);
  }//for

  return i;
    
}//serv_rs232_envia_amostras_gravadas_eeprom()

void serv_adcon_amostra_print(void) {
  lcd_clear();
  
  for (uint8_t i = 0; i < cfg_quant_sensores_atual; i++) {
    uint16_t x = adcon_leitura_sensor(i);
    #ifdef _LM35_
      serv_adcon_print(x, i);
    #endif
    #ifdef _MQ_
      mq_mostra(x, i);
    #endif
  }//for
}//serv_adcon_amostra_print()

void serv_adcon_amostra_print_grava(void) {
  uint16_t t_int;
  uint16_t maior, menor;
  uint8_t  qtd_leituras;
  uint16_t valores_leitura[CFG_QUANT_SENSORES_MAX];

   menor        = adcon_leitura_min;
   maior        = adcon_leitura_max;
   qtd_leituras = adcon_quant_leituras_gravadas;
   
  lcd_clear();
  if (qtd_leituras < ADCON_QTD_MAX_LEITURAS) {
    //Faz a leitura de todos os sensores e grava na EEPROM:
    for (uint8_t i = 0; i < cfg_quant_sensores_atual; i++) {
      t_int = adcon_leitura_sensor(i);
      if (t_int < menor) {
        menor = t_int;
      }
      if (t_int > maior) {
        maior = t_int;
      }
      valores_leitura[i] = t_int;
      qtd_leituras++;
      serv_adcon_print(t_int, i);
    } // for
    
    //Grava valores lidos na EEPROM.
    for (uint8_t i = 0; i < cfg_quant_sensores_atual; i++) {
      eeprom_grava_word(EEPROM_END_INICIO_AMOSTRAS + (adcon_quant_leituras_gravadas * 2 + i*2), valores_leitura[i]);
    }
    //Atualiza variáveis globais e grava na EEPROM.
    adcon_quant_leituras_gravadas = qtd_leituras;
    eeprom_write(EEPROM_END_QTDE_LEITURAS, adcon_quant_leituras_gravadas);
    if (menor < adcon_leitura_min) {
      adcon_leitura_min = menor;
      eeprom_grava_word(EEPROM_END_LEITURA_MIN, adcon_leitura_min);
    }
    if (maior > adcon_leitura_max) {
      adcon_leitura_max = maior;
      eeprom_grava_word(EEPROM_END_LEITURA_MAX, adcon_leitura_max);    
    }
    
  }//if (qtd_leituras < ADCON_QTD_MAX_LEITURAS)
  else {
    //Desabilita a interrupção do Timer0 para parar as leituras.
    INTCONbits.T0IE = 0;
    lcd_puts("Memoria Cheia");
  } //else
} //serv_adcon_amostra_print_grava()

void serv_adcon_print(uint16_t valor_sensor, uint8_t num_sensor) {
  //Desta maneira deu problemas no display:
  //char temp_str2[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
  char temp_str[9] = {0};
  #ifdef _LM35_
    div_t temp_div;
    temp_div =  div((int16_t)valor_sensor, 10);
    //alexdg 2023-09-29: Para diminuir as dependências, esta linha foi simplificada,
    //assim este módulo não depende mais do módulo "menu_principal.h"
    //sprintf(temp_str, "%d=%d.%d", menu_cfg_quant_sensores.itens[num_sensor].i_value, temp_div.quot, temp_div.rem);
    sprintf(temp_str, "%d=%d.%d", num_sensor+1, temp_div.quot, temp_div.rem);
  #endif
  #ifdef _MQ_
    //uint16_t ppm = potencia(t_int);
    //uint16_t ppm;
    //double yk;
    //yk = (mq_gl_r0 / (t_int * K));
    ppm = (uint16_t) pow(yk, INV_N);
    //mq_mostra(ppm, 0);
    //mq_mostra(t_int, 0);
    //lcd_clear();
    sprintf(temp_str, "ppm=%u", ppm);
  #endif
  lcd_goto_sensor(num_sensor);
  
  lcd_puts(temp_str);
}//serv_adcon_print()