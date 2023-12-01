//===== Includes =============================================================
//#include <eeprom_routines.h>  //eeprom_write()
#include <xc.h>
#include <stdlib.h>    //div_t
#include <stdio.h>     //sprintf

#include "serv_adcon.h"
#include "adcon.h"
#include "eeprom.h"
#include "lcd.h"
#include "rs232.h"

//Indica se está em uma sessão de monitora_grava.
//Quando a EEPROM estiver cheia, esta variável será colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta variável.
uint8_t serv_adcon_monitora_grava;

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variáveis Públicas ===================================================

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variáveis Privadas ===================================================

//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

void serv_adcon_amostra_print(void) {
  lcd_clear();
  
  for (uint8_t index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
    //Efetua leitura do sensor.
    uint16_t valor_sensor = adcon_leitura_sensor(index_sensor);
    
    #ifdef _LM35_
    //Imprime valor no display:
    //serv_adcon_print_leitura(valor_sensor, index_sensor);
    //MACRO: serv_adcon_print_leitura(valor_sensor,index_sensor)
    char temp_str[17] = {0};
    #ifdef _LM35_
      div_t temp_div;
      temp_div =  div((int16_t)valor_sensor, 10);
      sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
    #endif
    lcd_goto_sensor(index_sensor);
    lcd_puts(temp_str);
    //FIM MACRO.
    
    //Envia serial.
    sprintf(temp_str, "%d.%d,", temp_div.quot, temp_div.rem);
    rs232_envia_string(temp_str);
    
    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  uint16_t valor_sensor = x;
  uint8_t num_sensor = i;
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
     */
    
    
    
    
    #endif
    #ifdef _MQ_
      mq_mostra(x, i);
    #endif
  }//for
  
  rs232_envia_string("\n");
  
}//serv_adcon_amostra_print()

 /**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem (lê os sensores), imprime os valores no display e
 * grava os valores na EEPROM.
 * Atualiza a variável global adcon_quant_amostras_gravadas e grava-a na EEPROM.
 * Pode atualizar as variáveis globais adcon_leitura_min e adcon_leitura_max,
 * gravando-as também na EEPROM.
 * @return Retorna a quantidade de valores que foram lidos ou 0 caso não tenha espaço na EEPROM para gravar. 
 * Se não houver espaço na EEPROM, quem chamou esta função será responsável por interromper o monitoramento.
 */
uint8_t serv_adcon_amostra_print_grava(void) {
  uint16_t valor_sensor;
  uint16_t maior, menor;
  uint8_t  qtd_leituras;
  uint16_t valores_leitura[ADCON_CFG_QUANT_SENSORES_MAX];

   menor        = adcon_leitura_min;
   maior        = adcon_leitura_max;
   qtd_leituras = adcon_quant_leituras_gravadas;
   
  lcd_clear();
  
  //Verifica se tem espaço na EEPROM para armazenar as leituras.
  //Se tiver, então faz as leituras e armazena na EEPROM.
  
  if (qtd_leituras <= (ADCON_QTD_MAX_LEITURAS-adcon_cfg_quant_sensores_atual) ) {
    //Faz a leitura de todos os sensores e grava na EEPROM:
    uint8_t index_sensor=0;
    for (index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
      valor_sensor = adcon_leitura_sensor(index_sensor);
      if (valor_sensor < menor) {
        menor = valor_sensor;
      }
      if (valor_sensor > maior) {
        maior = valor_sensor;
      }
      valores_leitura[index_sensor] = valor_sensor;
      qtd_leituras++;
      
      //Imprime valor no display:
      //serv_adcon_print_leitura(valor_sensor, index_sensor);
      //MACRO: serv_adcon_print_leitura(valor_sensor,index_sensor)
      char temp_str[17] = {0};
      #ifdef _LM35_
        div_t temp_div;
        temp_div =  div((int16_t)valor_sensor, 10);
        sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      #endif
      lcd_goto_sensor(index_sensor);
      lcd_puts(temp_str);
      //FIM MACRO.
      
      //Envia serial.
      sprintf(temp_str, "%d.%d,", temp_div.quot, temp_div.rem);
      rs232_envia_string(temp_str);

    } // for
    
    rs232_envia_string("\n");
    
    //Grava valores lidos na EEPROM.
    for (uint8_t i = 0; i < adcon_cfg_quant_sensores_atual; i++) {
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
    
    //Retorna a quantidade de valores lidos, ou seja, o valor de index_sensor estará acrescido de 1 quando sair do laço for.
    return index_sensor;
    
  }//if (qtd_leituras < ADCON_QTD_MAX_LEITURAS)
  
  else {
    //Não tem espaço na EEPROM.
    //Desabilita a interrupção do Timer0 para parar as leituras.
    //Retorna 0 (zero) para indicar que nenhum valor foi lido pois 
    //não havia espaço para gravar na EEPROM.
    //INTCONbits.T0IE = 0; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    serv_adcon_monitora_grava = 0;
    lcd_puts("Mem.Cheia");
    return 0;
  } //else
  
} //serv_adcon_amostra_print_grava()

/*
void serv_adcon_print_leitura(uint16_t valor_sensor, uint8_t num_sensor) {
  //Desta maneira deu problemas no display:
  //char temp_str2[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
  char temp_str[17] = {0};
  #ifdef _LM35_
    div_t temp_div;
    temp_div =  div((int16_t)valor_sensor, 10);
    //alexdg 2023-09-29: Para diminuir as dependências, esta linha foi simplificada,
    //assim este módulo não depende mais do módulo "menu_principal.h"
    //sprintf(temp_str, "%d=%d.%d", menu_cfg_quant_sensores.itens[num_sensor].i_value, temp_div.quot, temp_div.rem);
    
    //sprintf(temp_str, "%d", num_sensor+1); //<<<<<<<<<<<<<<<<<<<<<<<<<<
    sprintf(temp_str, "%d=%d.%d", num_sensor+1, temp_div.quot, temp_div.rem);
    //printf("%s", temp_str);
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
}//serv_adcon_print_leitura()
*/

/**
 * Mostra no LCD os valores de uma amostra da EEPROM. O parâmetro indice_amostra
 * é o índice da amostra que será buscada da EEPROM para ser mostrada no LCD.
 * Se não houver amostras suficientes, mostra mensagem adequada no LCD.
 * @param indice_amostra índice da amostra que será buscada da EEPROM.
 */
void serv_adcon_print_amostra_eeprom(uint8_t indice_amostra) {
  uint8_t endereco;
  uint16_t valor_sensor;
  
  lcd_clear();
  
  if (adcon_quant_leituras_gravadas == 0) {
    lcd_puts("Nenhum Dado");
  }
  
  //Caso tenha sido modificada a quantidade de sensores.
  else if (adcon_quant_leituras_gravadas < adcon_cfg_quant_sensores_atual) {
    lcd_puts("Erro qt dados");
  }
  
  else {
    endereco = EEPROM_END_INICIO_AMOSTRAS + (indice_amostra * adcon_cfg_quant_sensores_atual * 2);
    //Mostrar no LCD os valores de uma amostra (todos os sensores) gravada na EEPROM:
    for (uint8_t index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
      valor_sensor = eeprom_le_word(endereco);
      
      //Imprime valor no display:
      //serv_adcon_print_leitura(valor_sensor, index_sensor);
      //MACRO: serv_adcon_print_leitura(valor_sensor,index_sensor)
      char temp_str[17] = {0};
      #ifdef _LM35_
        div_t temp_div;
        temp_div =  div((int16_t)valor_sensor, 10);
        sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      #endif
      lcd_goto_sensor(index_sensor);
      lcd_puts(temp_str);
      //FIM MACRO.
      
      //sprintf(temp_str, "%d=%d.%d", index_sensor+1, valor_sensor / 10, valor_sensor % 10);
      
      endereco = endereco + 2; //cada leitura ocupa 2 bytes.
    }//for
  }//else
  
}//serv_adcon_print_amostra_eeprom()

/**
 * Mostra no LCD os valores Max e Min. Se não houver amostras armazenadas na EEPROM, 
 * mostra mensagem adequada no LCD.
 */
void serv_adcon_print_max_min(void) {
  char tmp[17] = {0}; //warning do compilador
  lcd_clear();
  if (adcon_quant_leituras_gravadas == 0) {
    lcd_puts("Nenhum Dado");
  }
  else {
    sprintf(tmp, "Max=%d.%d", adcon_leitura_max / 10, adcon_leitura_max % 10);
    lcd_puts(tmp);
    lcd_goto(2, 0);
    sprintf(tmp, "Min=%d.%d", adcon_leitura_min / 10, adcon_leitura_min % 10);
    lcd_puts(tmp);
  }
}//serv_adcon_print_max_min()

/**
 * Verifica se um indice está dentro da faixa de valores válidos para as leituras
 * que estão armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_leituras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice a ser verificado.
 * @return retorna um (1) se o indice está na faixa válida ou zero (0) se
 * não estiver na faixa válida.
 */
uint8_t serv_adcon_testa_indice_amostra_valido(uint8_t indice) {
  //Fiz casting para 16bits para remover lbdiv do assembly mas não removeu.
  //Antes:  RAM=312 Program=7430
  //Depois: RAM=312 Program=7442
  //uint16_t ui16 = (uint16_t) ( (uint16_t)adcon_quant_leituras_gravadas) / ((uint16_t)adcon_cfg_quant_sensores_atual);
  //if (indice >=0 && indice <= ui16 ) {
  if ( (indice >=0) && ( indice < (adcon_quant_leituras_gravadas/adcon_cfg_quant_sensores_atual - 1) ) ) {
    return 1;
  }
  else {
    return 0;
  }
}//serv_adcon_testa_indice_amostra

/* Verifica se já passou a contagem de tempo para efetuar uma amostra.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se já passou a contagem ou zero (0) se ainda não passou.
 */
uint8_t serv_adcon_testa_timer_tempo_amostra(uint16_t count) {
  if (count >= adcon_cfg_tempo_amostra_atual) {
    return 1;
  }
  else {
    return 0;
  }
}

void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores) {
  adcon_cfg_quant_sensores_atual = quant_sensores;
  eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, adcon_cfg_quant_sensores_atual);
}//serv_adcon_set_quant_sensores_atual()

void serv_adcon_set_tempo_amostra_atual(uint16_t tempo_amostra) {
  adcon_cfg_tempo_amostra_atual = tempo_amostra;
  eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, adcon_cfg_tempo_amostra_atual);
}//serv_adcon_set_tempo_amostra_atual()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

