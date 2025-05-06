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
#include "versao.h"
#include "base_adcon.h"
#include "base_eeprom.h"
#include "base_lcd.h"
#include "base_rs232.h"

//Indica se est� em uma sess�o de monitora_grava.
//Quando a EEPROM estiver cheia, esta vari�vel ser� colocada em zero (0),
//assim, quem estiver efetuando o monitoramento deve checar esta vari�vel.
uint8_t serv_adcon_bol_monitora_grava;

//Indica a quantidade de sensores a serem mostrados em uma linha do LCD.
//Valor deve estar entre 1 e 4.
uint8_t serv_adcon_quant_sensores_por_linha_lcd;

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Vari�veis P�blicas ===================================================

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Vari�veis Privadas ===================================================

//Guarda os valores das amostras efetuadas em uma aquisi��o.
//A fun��o serv_adcon_aquisicao() grava os valores nesse array.
//A fun��o serv_adcon_print() mostra no LCD os valores que est�o nesse array.
static uint16_t serv_adcon_aquisicao_valores[ADCON_QUANT_MAX_SENSORES];

//===== Declara��o das Fun��es Privadas ======================================

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

/****************************************************************************
* Faz uma aquisi��o (faz amostragem dos sensores) e armazena os valores no array
* serv_adcon_aquisicao_valores[].
 ***************************************************************************/
void serv_adcon_aquisicao_amostrar(void) {
  for (uint8_t index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
    //Efetua amostragem do sensor.
    uint16_t valor_sensor = adcon_amostra_sensor(index_sensor);
    serv_adcon_aquisicao_valores[index_sensor] = valor_sensor;
  }//for
}//serv_adcon_aquisicao_amostrar()

/****************************************************************************
 * Imprime no LCD os valores que est�o no array serv_adcon_aquisicao_valores[].
 * @param quant_sensores quantidade de sensores a imprimir.
 ***************************************************************************/
void serv_adcon_aquisicao_imprimir(uint8_t quant_sens) {
  uint16_t valor_sensor;
  lcd_clear();
  for (uint8_t index_sensor = 0; index_sensor < quant_sens; index_sensor++) {
    
    #if defined(_HARDWARE_2013_)

      //Imprime valor no display:
      //serv_adcon_print_leitura(valor_sensor, index_sensor);
      //MACRO: serv_adcon_print_leitura(valor_sensor,index_sensor)
      valor_sensor = serv_adcon_aquisicao_valores[index_sensor];
      char temp_str[17] = {0};
      div_t temp_div;
      temp_div =  div((int16_t)valor_sensor, 10);
      //sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      
      lcd_goto_sensor(index_sensor);
      lcd_puts(temp_str);

    #elif defined(_HARDWARE_2016_)
      //Imprime valor absoluto
      valor_sensor = serv_adcon_aquisicao_valores[index_sensor];
      char temp_str[17] = {0};
      adcon_binario_para_valor(valor_sensor, temp_str);
      lcd_goto_sensor(index_sensor);
      lcd_puts(temp_str);

      //Imprime como se fosse temperatura:
      /*
      valor_sensor = serv_adcon_amostras[index_sensor];
      char temp_str[17] = {0};
      div_t temp_div;
      temp_div =  div((int16_t)valor_sensor, 10);
      //sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      sprintf(temp_str, "%d=%d.%d", index_sensor+1, temp_div.quot, temp_div.rem);
      lcd_goto_sensor(index_sensor);
      lcd_puts(temp_str);
      */

      //Imprimir valor em PPM:
      //uint16_t ppm = potencia(t_int);
      //uint16_t ppm;
      //double yk;
      //yk = (mq_gl_r0 / (t_int * K));
      //ppm = (uint16_t) pow(yk, INV_N);
      //mq_mostra(ppm, 0);
      //mq_mostra(t_int, 0);
      //lcd_clear();
      //sprintf(temp_str, "ppm=%u", ppm);
      
    #endif
      
    //Envia serial.
    //sprintf(temp_str, "%d.%d,", temp_div.quot, temp_div.rem);
    //rs232_envia_string(temp_str);

  }//for
  
    //rs232_envia_string("\n");
  
    //Se estiver em modo __DEBUG ent�o envia um comando para troca de linha, sen�o
    //a impress�o do valor anterior ficar� na mesma linha na janela "UART 1 Output" 
    //da IDE MPLABX.
    //Isto s� dever� ser utilizado em modo DEBUG na IDE MPLABX.
    #ifdef __DEBUG
    while (!TXIF) {
      continue;
    }      
    TXREG = '\n';
    #endif    

}//serv_adcon_aquisicao_imprimir())

/****************************************************************************
* Faz uma aquisi��o (faz amostragem dos sensores), imprime os valores no display e
* grava os valores na EEPROM.
* Atualiza a vari�vel global adcon_quant_amostras_gravadas e grava-a na EEPROM.
* Pode atualizar as vari�veis globais adcon_amostra_min e adcon_amostra_max,
* gravando-as tamb�m na EEPROM.
* @return Retorna a quantidade de amostras que foram lidas ou 0 caso n�o tenha espa�o na EEPROM para gravar. 
* Se n�o houver espa�o na EEPROM, a fun��o sinaliza colocando zero na vari�vel global serv_adcon_monitora_grava.
* Quem chamou esta fun��o ser� respons�vel por interromper o monitoramento.
*****************************************************************************/
uint8_t serv_adcon_aquisicao_amostrar_imprimir_gravar(void) {
  uint16_t amostra_sensor;
  uint16_t maior, menor;
  uint8_t  qtd_amostras;

  menor        = adcon_amostra_min;
  maior        = adcon_amostra_max;
  qtd_amostras = adcon_quant_amostras_gravadas;
   
  lcd_clear();
  
  //Verifica se tem espa�o na EEPROM para armazenar as amostras da aquisi��o.
  //Se tiver, ent�o faz as leituras e armazena na EEPROM.
  
  if (qtd_amostras > (ADCON_QTD_MAX_AMOSTRAS-adcon_cfg_quant_sensores_atual) ) {
    //N�o tem espa�o na EEPROM.
    //Sinaliza que a se��o de monitoravmento/grava��o deve ser finalizada.
    //Imprime mensagem no LCD.
    //Retorna 0 (zero) para indicar que nenhum valor foi amostrado pois 
    //n�o havia espa�o para gravar na EEPROM.
    serv_adcon_bol_monitora_grava = 0;
    lcd_puts("Mem.Cheia");
    return 0;
  }
  
  
  else if (adcon_quant_sensores_aquisitados>0 && adcon_quant_sensores_aquisitados!=adcon_cfg_quant_sensores_atual) {
    lcd_puts("Erro: qt. sens.");
    return 0;
  }
  
  else  {
    
    //Faz a amostragem de todos os sensores e mostra no LCD.
    serv_adcon_aquisicao_amostrar();
    serv_adcon_aquisicao_imprimir(adcon_cfg_quant_sensores_atual);
    
    //Verifica m�ximo e m�nimo:
    uint8_t index_sensor=0;
    for (index_sensor = 0; index_sensor < adcon_cfg_quant_sensores_atual; index_sensor++) {
      amostra_sensor = serv_adcon_aquisicao_valores[index_sensor];
      if (amostra_sensor < menor) {
        menor = amostra_sensor;
      }
      if (amostra_sensor > maior) {
        maior = amostra_sensor;
      }
    } // for
    
    qtd_amostras += adcon_cfg_quant_sensores_atual;
    
    //Grava na EEPROM as amostras efetuadas.
    for (uint8_t i = 0; i < adcon_cfg_quant_sensores_atual; i++) {
      eeprom_grava_word(EEPROM_END_INICIO_AMOSTRAS + (adcon_quant_amostras_gravadas * 2 + i*2), serv_adcon_aquisicao_valores[i]);
    }
    
    //Atualiza vari�veis globais e grava na EEPROM.
    adcon_quant_amostras_gravadas = qtd_amostras;
    eeprom_write(EEPROM_END_QTDE_AMOSTRAS, adcon_quant_amostras_gravadas);
    adcon_quant_sensores_aquisitados = adcon_cfg_quant_sensores_atual;
    eeprom_write(EEPROM_END_QTDE_SENSORES_AMOSTRADOS, adcon_quant_sensores_aquisitados);
    if (menor < adcon_amostra_min) {
      adcon_amostra_min = menor;
      eeprom_grava_word(EEPROM_END_LEITURA_MIN, adcon_amostra_min);
    }
    
    if (maior > adcon_amostra_max) {
      adcon_amostra_max = maior;
      eeprom_grava_word(EEPROM_END_LEITURA_MAX, adcon_amostra_max);    
    }
    
    //Retorna a quantidade de amostas lidas (que � igual a quantidade de sensores).
    return adcon_cfg_quant_sensores_atual;
    
  }//else 
  
} //serv_adcon_aquisicao_amostrar_imprimir_gravar()

/****************************************************************************
* Mostra no LCD os valores de uma aquisi��o da EEPROM. 
* O par�metro indice_aquisicao � o �ndice da aquisi��o que ser� buscada da EEPROM 
* para ser mostrada no LCD.
* Se n�o houver aquisi��es, mostra mensagem adequada no LCD.
* @param indice_aquisicao �ndice da aquisi��o que ser� buscada da EEPROM.
*****************************************************************************/
void serv_adcon_aquisicao_imprimir_da_eeprom(uint8_t indice_aquisicao) {
  uint8_t endereco;
  uint16_t valor_amostra;
  
  lcd_clear();
  
  if (adcon_quant_amostras_gravadas == 0) {
    lcd_puts("Nenhum Dado");
    return;
  }
  
  //Caso tenha sido modificada a quantidade de sensores.
  //else if (adcon_quant_amostras_gravadas < adcon_cfg_quant_sensores_atual) {
  //  lcd_puts("Erro qt dados");
  //  return;
 // }
  
  else {
    endereco = EEPROM_END_INICIO_AMOSTRAS + (indice_aquisicao * adcon_quant_sensores_aquisitados * 2);
    //Pega a aquisi��o (amostras) da EEPROM e grava no array serv_adcon_amostras[].
    for (uint8_t index_sensor = 0; index_sensor < adcon_quant_sensores_aquisitados; index_sensor++) {
      valor_amostra = eeprom_le_word(endereco);
      serv_adcon_aquisicao_valores[index_sensor] = valor_amostra;
      endereco = endereco + 2; //cada amostra ocupa 2 bytes.
    }//for
    //Mostra no LCD as amostras.
    serv_adcon_aquisicao_imprimir(adcon_quant_sensores_aquisitados);
  }//else
  
}//serv_adcon_aquisicao_imprimir_da_eeprom()

/****************************************************************************
* Mostra no LCD os valores Max e Min. Se n�o houver amostras armazenadas na EEPROM, 
* mostra mensagem adequada no LCD.
****************************************************************************/
void serv_adcon_print_max_min(void) {
  char tmp[17] = {0}; //warning do compilador
  char tmp_aux[17] = {0}; //warning do compilador
  lcd_clear();
  if (adcon_quant_amostras_gravadas == 0) {
    lcd_puts("Nenhum Dado");
  }
  else {
    strcpy(tmp_aux,"Max=");
    adcon_binario_para_valor(adcon_amostra_max, tmp);
    strcat(tmp_aux, tmp);
    lcd_puts(tmp_aux);
    
    lcd_goto(2, 0);
    strcpy(tmp_aux,"Min=");
    adcon_binario_para_valor(adcon_amostra_min, tmp);
    strcat(tmp_aux, tmp);
    lcd_puts(tmp_aux);
  }
}//serv_adcon_print_max_min()

/**
 * Verifica se um indice est� dentro da faixa de valores v�lidos para as aquisi��es
 * que est�o armazenadas na EEPROM. Indice deve estar entre 
 * zero e adcon_quant_amostras_gravadas / adcon_cfg_quant_sensores_atual
 * @param indice indice da aquisi��o a ser verificada.
 * @return retorna um (1) se o indice est� na faixa v�lida ou zero (0) se
 * n�o estiver na faixa v�lida.
 */
uint8_t serv_adcon_aquisicao_testa_indice_valido(uint8_t indice) {
  //Fiz casting para 16bits para remover lbdiv do assembly mas n�o removeu.
  //Antes:  RAM=312 Program=7430
  //Depois: RAM=312 Program=7442
  //uint16_t ui16 = (uint16_t) ( (uint16_t)adcon_quant_leituras_gravadas) / ((uint16_t)adcon_cfg_quant_sensores_atual);
  //if (indice >=0 && indice <= ui16 ) {
  if ( (indice >=0) && ( indice < (adcon_quant_amostras_gravadas/adcon_quant_sensores_aquisitados - 1) ) ) {
    return 1;
  }
  else {
    return 0;
  }
}//serv_adcon_aquisicao_testa_indice_valido()

/* Verifica se j� passou a contagem de tempo para efetuar uma aquisi��o.
 * @param count valor do timer a ser verificado.
 * @return retorna um (1) se j� passou a contagem ou zero (0) se ainda n�o passou.
 */
uint8_t serv_adcon_testa_contagem_timer(uint16_t count) {
  if (count >= adcon_cfg_tempo_aquisicao_atual) {
    return 1;
  }
  else {
    return 0;
  }
}//serv_adcon_testa_contagem_timer()

/**
 * Atualiza a vari�vel adcon_cfg_quant_sensores_atual para o valor quant_sensores e
 * grava tamb�m na EEPROM.
 * @param quant_sensores valor a ser atualizado na vari�vel adcon_cfg_quant_sensores_atual.
 */
void serv_adcon_set_quant_sensores_atual(uint8_t quant_sensores) {
  adcon_cfg_quant_sensores_atual = quant_sensores;
  eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, adcon_cfg_quant_sensores_atual);
}//serv_adcon_set_quant_sensores_atual()

/**
 * Configura o tempo entre aquisi��es. 
 * Atualiza a vari�vel adcon_cfg_tempo_aquisicao_atual para o valor tempo_aquisicao e
 * grava tamb�m na EEPROM.
 * @param tempo_aquisicao novo valor para o tempo entre aquisi��es.
 */
void serv_adcon_set_tempo_aquisicao_atual(uint16_t tempo_amostra) {
  adcon_cfg_tempo_aquisicao_atual = tempo_amostra;
  eeprom_grava_word(EEPROM_END_TEMPO_AQUISICAO, adcon_cfg_tempo_aquisicao_atual);
}//serv_adcon_set_tempo_aquisicao_atual()

/**
 * Inicializa as configura��es da EPRROM com os valores dafault, 
 * gravando tamb�m a chave de inicializa��o.
 */
void serv_adcon_eeprom_inicializa_configuracoes(void) {
    adcon_cfg_quant_sensores_atual      = 1;
    eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, adcon_cfg_quant_sensores_atual);
    
    adcon_quant_sensores_aquisitados = 0;
    eeprom_write(EEPROM_END_QTDE_SENSORES_AMOSTRADOS, adcon_quant_sensores_aquisitados);
    
    adcon_cfg_tempo_aquisicao_atual       = ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO;
    eeprom_grava_word(EEPROM_END_TEMPO_AQUISICAO, adcon_cfg_tempo_aquisicao_atual);

    //A chave de inicializacao tem que ser a ultima a ser gravada (OBRIGATORIAMENTE),
    //pois, parece que quando o hardware gravador termina de efetuar a gravacao,
    //o microcontrolador eh ligado por um breve momento, enquanto ha energia eletrica
    //residual nos capacitores do gravador.
    //<<<<<<< isto ainda precisa ser verificado <<<<<<<<<<<<<<<<
    eeprom_write(EEPROM_END_CHAVE_INICIALIZACAO, EEPROM_VALOR_CHAVE_INICIALIZACAO);
}//serv_adcon_eeprom_inicializa_configuracoes()

/**
 * Le as configura��es da EEPROM e atualiza as vari�veis globais. 
 */
void serv_adcon_eeprom_le_configuracoes(void) {
  adcon_quant_amostras_gravadas   = eeprom_read(EEPROM_END_QTDE_AMOSTRAS);
  adcon_cfg_quant_sensores_atual  = eeprom_read(EEPROM_END_QTDE_SENSORES_ATUAL);
  adcon_quant_sensores_aquisitados = eeprom_read(EEPROM_END_QTDE_SENSORES_AMOSTRADOS);
  adcon_cfg_tempo_aquisicao_atual = eeprom_le_word(EEPROM_END_TEMPO_AQUISICAO);
  adcon_amostra_min               = eeprom_le_word(EEPROM_END_LEITURA_MIN);
  adcon_amostra_max               = eeprom_le_word(EEPROM_END_LEITURA_MAX);
  //uint8_t qtd_sens     = eeprom_read(EEPROM_END_QTDE_SENSORES_AMOSTRADOS);
}//serv_adcon_eeprom_le_configuracoes()

/**
 * Limpa os seguintes dados da EEPROM:
 * - as aquisi��es.
 * - os valores m�nimo e m�ximo.
 */
void serv_adcon_eeprom_limpa_dados(void) {
    adcon_quant_amostras_gravadas = 0;
    eeprom_write(EEPROM_END_QTDE_AMOSTRAS, adcon_quant_amostras_gravadas);
    
    adcon_quant_sensores_aquisitados = 0;
    eeprom_write(EEPROM_END_QTDE_SENSORES_AMOSTRADOS, adcon_quant_sensores_aquisitados);
    
    //Grava em adcon_amostra_min o valor MAXIMO poss�vel.
    adcon_amostra_min = ADCON_VALOR_MAXIMO_AMOSTRA;
    eeprom_grava_word(EEPROM_END_LEITURA_MIN, ADCON_VALOR_MAXIMO_AMOSTRA);
    
    //Grava em adcon_amostra_max o valor M�NIMO poss�vel.
    adcon_amostra_max = ADCON_VALOR_MINIMO_AMOSTRA;
    eeprom_grava_word(EEPROM_END_LEITURA_MAX, ADCON_VALOR_MINIMO_AMOSTRA);

    //Limpa toda a area das amostras.
    for (uint8_t i = 0; i < ADCON_QTD_MAX_AMOSTRAS * 2; i++) {
      eeprom_write(EEPROM_END_INICIO_AMOSTRAS + i, 0);
    }  
}//serv_adcon_eeprom_limpa_dados()

/**
* Envia pela RS232 todas as amostras que est�o gravadas na EEPROM e d� mensagem adequada no LCD.
* @return quantidade de bytes transmitidos.
*/
uint8_t serv_adcon_rs232_envia_amostras_gravadas_eeprom(void) {
  //uint8_t dado;
  uint16_t valor_amostra;
  char str_valor[7] = {0}; //5 caracteres para o valor + 2 para \n = 7.
  char str_linha[25];      //5 caracteres para cada sensor (5x4=20), 3 virgulas, 2 para \n. Total=20+3+2=25.
  char *p_str_valor;
  char *p_str_linha;
  
  lcd_clear();
  lcd_puts("Env. serial...");

  //2400 bauds = (0,000416667 s) * (10 bits em cada transmissao serial) = 4.16 ms
    
  //O java trabalha com inteiros de 16 bits com sinal (little endian) ent�o
  //para facilitar todos os dados ser�o enviados como words de 16 bits.
    
  //Envia a informa��o da quantidade de sensores atualmente configurados (Big Endian).
  //rs232_envia_byte(0);
  //rs232_envia_byte(adcon_cfg_quant_sensores_atual);  
  
  uint8_t i;
  p_str_linha = str_linha;
  //Envia pela RS232 as amostras que est�o armazenadas na EEPROM.    
  uint8_t num_sensor = 0;
  for (i=0; i < adcon_quant_amostras_gravadas; i = i + 2) {
    num_sensor++;
    /*
    //Pega byte mais significativo.
    valor_amostra = eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i);
    valor_amostra = valor_amostra << 8;
    //Pega o byte menos significativo.
    valor_amostra = valor_amostra + eeprom_read(EEPROM_END_INICIO_AMOSTRAS + i + 1);
    */
    
    valor_amostra = eeprom_le_word(EEPROM_END_INICIO_AMOSTRAS + i);
    
    //Transforma valor para string.
    adcon_binario_para_valor(valor_amostra, str_valor);
    
    //Concatena na linha e adiciona uma v�rgula.
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
    
}//serv_adcon_rs232_envia_amostras_gravadas_eeprom()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================

