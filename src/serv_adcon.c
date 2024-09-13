/*****************************************************************************
 * File:   serv_adcon.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================
//#include <eeprom_routines.h>  //eeprom_write()
#include <xc.h>
#include <stdlib.h>    //div_t
#include <stdio.h>     //sprintf
#include <string.h>     //strcat

#include "serv_adcon.h"
#include "base_adcon.h"
#include "base_eeprom.h"
#include "base_lcd.h"
#include "base_rs232.h"

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

/**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma aquisição (lê os sensores) e imprime os valores no display.
 */
void serv_adcon_aquisicao_print(void) {
  lcd_clear();
  
  for (uint8_t index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
    //Efetua leitura do sensor.
    uint16_t valor_sensor = adcon_amostra_sensor(index_sensor);
    
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
    
    //Envia serial.
    sprintf(temp_str, "%d.%d,", temp_div.quot, temp_div.rem);
    rs232_envia_string(temp_str);

    //Se estiver em modo __DEBUG então envia um comando para troca de linha, senão
    //a impressão do valor anterior ficará na mesma linha na janela "UART 1 Output" 
    //da IDE MPLABX.
    //Isto só deverá ser utilizado em modo DEBUG na IDE MPLABX.
    #ifdef __DEBUG
    while (!TXIF) {
      continue;
    }      
    TXREG = '\n';
    #endif    
    
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
  
}//serv_adcon_aquisicao_print()

/**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma aquisição (lê os sensores), imprime os valores no display e
 * grava os valores na EEPROM.
 * Atualiza a variável global adcon_quant_amostras_gravadas e grava-a na EEPROM.
 * Pode atualizar as variáveis globais adcon_amostra_min e adcon_amostra_max,
 * gravando-as também na EEPROM.
 * @return Retorna a quantidade de amostras que foram lidas ou 0 caso não tenha espaço na EEPROM para gravar. 
 * Se não houver espaço na EEPROM, quem chamou esta função será responsável por interromper o monitoramento.
  */
uint8_t serv_adcon_aquisicao_print_grava(void) {
  uint16_t amostra_sensor;
  uint16_t maior, menor;
  uint8_t  qtd_amostras;
  uint16_t valores_amostras[ADCON_CFG_QUANT_SENSORES_MAX];

   menor        = adcon_amostra_min;
   maior        = adcon_amostra_max;
   qtd_amostras = adcon_quant_amostras_gravadas;
   
  lcd_clear();
  
  //Verifica se tem espaço na EEPROM para armazenar as leituras.
  //Se tiver, então faz as leituras e armazena na EEPROM.
  
  if (qtd_amostras <= (ADCON_QTD_MAX_AMOSTRAS-adcon_cfg_quant_sensores_atual) ) {
    //Faz a amostragem de todos os sensores e grava na EEPROM:
    uint8_t index_sensor=0;
    for (index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
      amostra_sensor = adcon_amostra_sensor(index_sensor);
      if (amostra_sensor < menor) {
        menor = amostra_sensor;
      }
      if (amostra_sensor > maior) {
        maior = amostra_sensor;
      }
      valores_amostras[index_sensor] = amostra_sensor;
      qtd_amostras++;
      
      //Imprime amostra no display:
      //serv_adcon_print_leitura(valor_sensor, index_sensor);
      //MACRO: serv_adcon_print_leitura(valor_sensor,index_sensor)
      char temp_str[17] = {0};
      #ifdef _LM35_
        div_t temp_div;
        temp_div =  div((int16_t)amostra_sensor, 10);
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
      eeprom_grava_word(EEPROM_END_INICIO_AMOSTRAS + (adcon_quant_amostras_gravadas * 2 + i*2), valores_amostras[i]);
    }
    //Atualiza variáveis globais e grava na EEPROM.
    adcon_quant_amostras_gravadas = qtd_amostras;
    eeprom_write(EEPROM_END_QTDE_LEITURAS, adcon_quant_amostras_gravadas);
    if (menor < adcon_amostra_min) {
      adcon_amostra_min = menor;
      eeprom_grava_word(EEPROM_END_LEITURA_MIN, adcon_amostra_min);
    }
    if (maior > adcon_amostra_max) {
      adcon_amostra_max = maior;
      eeprom_grava_word(EEPROM_END_LEITURA_MAX, adcon_amostra_max);    
    }
    
    //Retorna a quantidade de amostas lidas, ou seja, o valor de index_sensor estará acrescido de 1 quando sair do laço for.
    return index_sensor;
    
  }//if (qtd_amostras < ADCON_QTD_MAX_AMOSTRAS)
  
  else {
    //Não tem espaço na EEPROM.
    //Desabilita a interrupção do Timer0 para parar as leituras.
    //Retorna 0 (zero) para indicar que nenhum valor foi amostrado pois 
    //não havia espaço para gravar na EEPROM.
    //INTCONbits.T0IE = 0; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    serv_adcon_monitora_grava = 0;
    lcd_puts("Mem.Cheia");
    return 0;
  } //else
  
} //serv_adcon_aquisicao_print_grava()

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
 * Mostra no LCD os valores de uma aquisição da EEPROM. O parâmetro indice_aquisicao
 * é o índice da aquisição que será buscada da EEPROM para ser mostrada no LCD.
 * Se não houver aquisições, mostra mensagem adequada no LCD.
 * @param indice_aquisicao índice da aquisição que será buscada da EEPROM.
 */
void serv_adcon_print_aquisicao_da_eeprom(uint8_t indice_aquisicao) {
  uint8_t endereco;
  uint16_t valor_amostra;
  
  lcd_clear();
  
  if (adcon_quant_amostras_gravadas == 0) {
    lcd_puts("Nenhum Dado");
  }
  
  //Caso tenha sido modificada a quantidade de sensores.
  else if (adcon_quant_amostras_gravadas < adcon_cfg_quant_sensores_atual) {
    lcd_puts("Erro qt dados");
  }
  
  else {
    endereco = EEPROM_END_INICIO_AMOSTRAS + (indice_aquisicao * adcon_cfg_quant_sensores_atual * 2);
    //Mostrar no LCD as amostras de uma aquisição (todos os sensores) gravada na EEPROM:
    for (uint8_t index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
      valor_amostra = eeprom_le_word(endereco);
      
      //Imprime amostra no display:
      //serv_adcon_print_leitura(valor_sensor, index_sensor);
      //MACRO: serv_adcon_print_leitura(valor_sensor,index_sensor)
      char temp_str[17] = {0};
      #ifdef _LM35_
        div_t temp_div;
        temp_div =  div((int16_t)valor_amostra, 10);
        sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      #endif
      lcd_goto_sensor(index_sensor);
      lcd_puts(temp_str);
      //FIM MACRO.
      
      //sprintf(temp_str, "%d=%d.%d", index_sensor+1, valor_sensor / 10, valor_sensor % 10);
      
      endereco = endereco + 2; //cada amostra ocupa 2 bytes.
    }//for
  }//else
  
}//serv_adcon_print_aquisicao_da_eeprom()

/**
 * Mostra no LCD os valores Max e Min. Se não houver amostras armazenadas na EEPROM, 
 * mostra mensagem adequada no LCD.
 */
void serv_adcon_print_max_min(void) {
  char tmp[17] = {0}; //warning do compilador
  lcd_clear();
  if (adcon_quant_amostras_gravadas == 0) {
    lcd_puts("Nenhum Dado");
  }
  else {
    sprintf(tmp, "Max=%d.%d", adcon_amostra_max / 10, adcon_amostra_max % 10);
    lcd_puts(tmp);
    lcd_goto(2, 0);
    sprintf(tmp, "Min=%d.%d", adcon_amostra_min / 10, adcon_amostra_min % 10);
    lcd_puts(tmp);
  }
}//serv_adcon_print_max_min()

/**
 * Verifica se um indice está dentro da faixa de valores válidos para as aquisições
 * que estão armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_amostras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice da aquisição a ser verificada.
 * @return retorna um (1) se o indice está na faixa válida ou zero (0) se
 * não estiver na faixa válida.
 */
uint8_t serv_adcon_testa_indice_aquisicao_valida(uint8_t indice) {
  //Fiz casting para 16bits para remover lbdiv do assembly mas não removeu.
  //Antes:  RAM=312 Program=7430
  //Depois: RAM=312 Program=7442
  //uint16_t ui16 = (uint16_t) ( (uint16_t)adcon_quant_leituras_gravadas) / ((uint16_t)adcon_cfg_quant_sensores_atual);
  //if (indice >=0 && indice <= ui16 ) {
  if ( (indice >=0) && ( indice < (adcon_quant_amostras_gravadas/adcon_cfg_quant_sensores_atual - 1) ) ) {
    return 1;
  }
  else {
    return 0;
  }
}//serv_adcon_testa_indice_aquisicao_valida

/* Verifica se já passou a contagem de tempo para efetuar uma aquisição.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se já passou a contagem ou zero (0) se ainda não passou.
 */
uint8_t serv_adcon_testa_timer_tempo_aquisicao(uint16_t count) {
  if (count >= adcon_cfg_tempo_aquisicao_atual) {
    return 1;
  }
  else {
    return 0;
  }
}//serv_adcon_testa_timer_tempo_aquisicao()

/**
 * Atualiza a variável adcon_cfg_quant_sensores_atual para o valor quant_sensores e
 * grava também na EEPROM.
 * @param quant_sensores valor a ser atualizado na variável adcon_cfg_quant_sensores_atual.
 */
void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores) {
  adcon_cfg_quant_sensores_atual = quant_sensores;
  eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, adcon_cfg_quant_sensores_atual);
}//serv_adcon_set_quant_sensores_atual()

/**
 * Configura o tempo entre aquisições. 
 * Atualiza a variável adcon_cfg_tempo_aquisicao_atual para o valor tempo_aquisicao e
 * grava também na EEPROM.
 * @param tempo_aquisicao novo valor para o tempo entre aquisições.
 */
void serv_adcon_set_tempo_aquisicao_atual(uint16_t tempo_amostra) {
  adcon_cfg_tempo_aquisicao_atual = tempo_amostra;
  eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, adcon_cfg_tempo_aquisicao_atual);
}//serv_adcon_set_tempo_aquisicao_atual()

/**
 * Inicializa as configurações da EPRROM com os valores dafault, 
 * gravando também a chave de inicialização.
 */
void serv_adcon_inicializa_configuracoes_eeprom(void) {
    adcon_cfg_quant_sensores_atual      = 1;
    eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, adcon_cfg_quant_sensores_atual);
    
    //cfg_quant_sensores_amostrados = 1;
    //eeprom_grava_word(EEPROM_END_QTDE_SENSORES_AMOSTRADOS, cfg_quant_sensores_amostrados);
    
    adcon_cfg_tempo_aquisicao_atual       = ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO;
    eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, adcon_cfg_tempo_aquisicao_atual);

    //A chave de inicializacao tem que ser a ultima a ser gravada (OBRIGATORIAMENTE),
    //pois, parece que quando o hardware gravador termina de efetuar a gravacao,
    //o microcontrolador eh ligado por um breve momento, enquanto ha energia eletrica
    //residual nos capacitores do gravador.
    //<<<<<<< isto ainda precisa ser verificado <<<<<<<<<<<<<<<<
    eeprom_write(EEPROM_END_CHAVE_INICIALIZACAO, EEPROM_VALOR_CHAVE_INICIALIZACAO);
}//serv_adcon_inicializa_configuracoes_eeprom()

/**
 * Le as configurações da EEPROM e atualiza as variáveis globais. 
 */
void serv_adcon_le_configuracoes_eeprom(void) {
  adcon_quant_amostras_gravadas = eeprom_read(EEPROM_END_QTDE_LEITURAS);
  adcon_cfg_quant_sensores_atual      = eeprom_read(EEPROM_END_QTDE_SENSORES_ATUAL);
  adcon_cfg_tempo_aquisicao_atual       = eeprom_le_word(EEPROM_END_TEMPO_AMOSTRAGEM);
  adcon_amostra_min             = eeprom_le_word(EEPROM_END_LEITURA_MIN);
  adcon_amostra_max             = eeprom_le_word(EEPROM_END_LEITURA_MAX);
  //uint8_t qtd_sens     = eeprom_read(EEPROM_END_QTDE_SENSORES_AMOSTRADOS);
}//serv_adcon_le_configuracoes_eeprom()

/**
 * Limpa os seguintes dados da EEPROM:
 * - as aquisições.
 * - os valores mínimo e máximo.
 */
void serv_adcon_limpa_dados_eeprom(void) {
    adcon_quant_amostras_gravadas = 0;
    eeprom_write(EEPROM_END_QTDE_LEITURAS, adcon_quant_amostras_gravadas);
    
    //Grava em adcon_amostra_min o valor MAXIMO possível.
    adcon_amostra_min = ADCON_VALOR_MAXIMO_AMOSTRA;
    eeprom_grava_word(EEPROM_END_LEITURA_MIN, ADCON_VALOR_MAXIMO_AMOSTRA);
    
    //Grava em adcon_amostra_max o valor MÍNIMO possível.
    adcon_amostra_max = ADCON_VALOR_MINIMO_AMOSTRA;
    eeprom_grava_word(EEPROM_END_LEITURA_MAX, ADCON_VALOR_MINIMO_AMOSTRA);

    //Limpa toda a area das amostras.
    for (uint8_t i = 0; i < ADCON_QTD_MAX_AMOSTRAS * 2; i++) {
      eeprom_write(EEPROM_END_INICIO_AMOSTRAS + i, 0);
    }  
}//serv_adcon_limpa_dados_eeprom()

/**
* Envia pela RS232 todas as amostras que estão gravadas na EEPROM e dá mensagem adequada no LCD.
* @return quantidade de bytes transmitidos.
*/
uint8_t serv_adcon_envia_rs232_amostras_gravadas_eeprom(void) {
  //uint8_t dado;
  uint16_t valor_amostra;
  char str_valor[7] = {0}; //5 caracteres para o valor + 2 para \n = 7.
  char str_linha[25];      //5 caracteres para cada sensor (5x4=20), 3 virgulas, 2 para \n. Total=20+3+2=25.
  char *p_str_valor;
  char *p_str_linha;
  
  lcd_clear();
  lcd_puts("Env. serial...");

  //2400 bauds = (0,000416667 s) * (10 bits em cada transmissao serial) = 4.16 ms
    
  //O java trabalha com inteiros de 16 bits com sinal (little endian) então
  //para facilitar todos os dados serão enviados como words de 16 bits.
    
  //Envia a informação da quantidade de sensores atualmente configurados (Big Endian).
  //rs232_envia_byte(0);
  //rs232_envia_byte(adcon_cfg_quant_sensores_atual);  
  
  uint8_t i;
  p_str_linha = str_linha;
  //Envia pela RS232 as amostras que estão armazenadas na EEPROM.    
  uint8_t num_sensor = 0;
  for (i=0; i < adcon_quant_amostras_gravadas; i = i + 2) {
    num_sensor++;
    //Pega byte mais significativo.
    valor_amostra = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
    valor_amostra = valor_amostra << 8;
    //Pega o byte menos significativo.
    valor_amostra = valor_amostra + eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
    div_t temp_div;
    temp_div =  div((int16_t)valor_amostra, 10);
    
    //Transforma valor para string.
    sprintf(str_valor, "%d.%d", temp_div.quot, temp_div.rem);
    //Concatena na linha e adiciona uma vírgula.
    p_str_valor = str_valor;
    while (*p_str_valor != 0) {
      *p_str_linha = *p_str_valor;
      p_str_linha++;
      p_str_valor++;
    }
    *p_str_linha = ',';
    p_str_linha++;
    *p_str_linha = 0;
    
    strcat(str_linha, str_valor);
    //strcat(str_linha, ",");
    if (num_sensor == adcon_cfg_quant_sensores_atual) {
      //Envia a linha contendo os 4 sensores.
      //strcat(str_linha, "\n");
      *p_str_linha = '\n';
      p_str_linha++;
      *p_str_linha = 0;
      rs232_envia_string(str_linha);
      num_sensor   = 0;
      str_linha[0] = 0; //esvazia a string.
      p_str_linha  = str_linha;
    }
  }//for
  
 /* for (i=0; i < adcon_quant_leituras_gravadas; i = i + 2) {
      //Envia o byte mais significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
      rs232_envia_byte(dado);
      //Envia o byte menos significativo.
      dado = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
      rs232_envia_byte(dado);
  }//for
*/
  
  lcd_clear();
  sprintf(str_linha, "%d b env.", i);
  lcd_puts(str_linha);

  return i;
    
}//serv_adcon_envia_rs232_amostras_gravadas_eeprom()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

