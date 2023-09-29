/* 
 * File:   main.c
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 15:57
 */

#include <xc.h>
#include <stdlib.h>

#include <stdio.h>

#include "main.h"
#include "adcon.h"
#include "botoes.h"
#include "eeprom.h"
#include "lcd.h"
#include "rs232.h"
#include "timer.h"
#include "versao.h"
#include "cfg_quant_sensores.h"
#include "cfg_tempo_amostra.h"

#include "xtal.h"
#include "menu_principal.h"

#ifdef _MODULO_NOVO_
  #pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

#ifdef _MODULO_ANTIGO_
  #pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

//Esta função é usada somente para marcação no Clock Stimulus
//void inicializado3(void){ };
    

#ifdef __DEBUG
//Esta macro é implementada somente em modo __DEBUG 
//para que a saída do printf() seja na UART do simulador.
void putch(char data) {
  while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
    continue;
  TXREG = data;                     // send one character
}
#endif

/**
 * Funcao principal: inicializa os dispositivos do PIC, configura a interrupcao global
 * e fica em while().
 * @return EXIT_SUCCESS
 */
int main(void) {
  //Marcará quando o equipamento finalizou a inicialização (inicializado = 1).
  //uint8_t inicializado = 0;
    
  #ifdef _MODULO_ANTIGO_
    TBotao option;
  #endif
  //rs232_init() precisa ser antes do printf() e antes do lcd_puts() pois 
  //em modo __DEUBG essas funções utilizam a UART.
  rs232_init();
  lcd_init();

  #ifdef __DEBUG
    //Somente para testar o printf().)
    printf("teste1\n");
    printf("teste2\n");
  #endif

  lcd_puts("11/09/2023 - v1.60-");
  #ifdef _MODULO_NOVO_
    lcd_puts("N");
  #endif
  #ifdef _MODULO_ANTIGO_
    lcd_puts("A");
  #endif
  //Antes escriva D (de DEBUG), mas esta diretiva foi modificada em 2023-08-21.
  #ifdef _ENVIA_DADOS_SERIAL_
    lcd_puts("S");
  #endif

  lcd_goto(2, 0);
  lcd_puts("Iniciando...");

  adcon_init();
  timer0_init();

  TXSTAbits.TXEN = 1;               // enable transmitter
  RCSTAbits.SPEN = 1; 

  //timer2_init();

  
  // the Power-up Timer (72 ms duration) prevents EEPROM write:
  // 140ms = tempo maximo do power-up:
  __delay_ms(200);

  //Testa se a EEPROM de dados já foi gravada pelo equipamento.
  //Se ainda não foi, então inicializa com os valores de configuração padrão.
  if (eeprom_read(EEPROM_END_CHAVE_INICIALIZACAO) != EEPROM_VALOR_CHAVE_INICIALIZACAO) {
    cfg_quant_sensores_atual      = 1;
    cfg_quant_sensores_amostrados = 1;
    cfg_tempo_amostra_atual        = CFG_TEMPO_AMOSTRA_1_SEGUNDO;
    eeprom_write(EEPROM_END_QTDE_AMOSTRAS, 0);
    eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, cfg_quant_sensores_atual);
    eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, cfg_tempo_amostra_atual);
    eeprom_grava_word(EEPROM_END_LEITURA_MIN, ADCON_VALOR_MAXIMO_LEITURA);
    eeprom_grava_word(EEPROM_END_LEITURA_MAX, ADCON_VALOR_MINIMO_LEITURA);

    //Limpa toda a area de amostras.
    for (uint8_t i = 0; i < ADCON_QTD_MAX_LEITURAS * 2; i++) {
      eeprom_write(i + EEPROM_END_INICIO_AMOSTRAS, 0);
    }
    //A chave de inicializacao tem que ser a ultima a ser gravada (OBRIGATORIAMENTE),
    //pois, parece que quando o hardware gravador termina de efetuar a gravacao,
    //o microcontrolador eh ligado por um breve momento, enquanto ha energia eletrica
    //residual nos capacitores do gravador.
    //<<<<<<< isto ainda precisa ser verificado <<<<<<<<<<<<<<<<
    eeprom_write(EEPROM_END_CHAVE_INICIALIZACAO, EEPROM_VALOR_CHAVE_INICIALIZACAO);

    //gl_item_menu_config_amostra = 1;

  }//if (eeprom_read(END_CHAVE_INICIALIZACAO) != VALOR_CHAVE)
  else {
    //Parece que aqui deveria ler os valores de configuração que estão na EEPROM de dados.<<<<<<<<<<<<<<<<<<<<

    adcon_quant_amostras_gravadas = eeprom_read(EEPROM_END_QTDE_AMOSTRAS);
    uint8_t qtd_sens     = eeprom_read(EEPROM_END_QTDE_SENSORES_AMOSTRADOS);
    menu_set_value_indexes(&menu_cfg_quant_sensores, qtd_sens);
    cfg_quant_sensores_atual = eeprom_read(EEPROM_END_QTDE_SENSORES_ATUAL);

  }//else (VALOR_CHAVE)
  
  //Inicializa os menus:
  menu_init(&menu_principal, menu_principal_itens, MENU_PRINCIPAL_TAM);
  menu_init(&menu_cfg_quant_sensores, menu_cfg_quant_sensores_itens, MENU_CFG_QUANT_SENSORES_TAM);
  menu_init(&menu_cfg_tempo_amostra, menu_cfg_tempo_amostra_itens, MENU_CFG_TEMPO_AMOSTRA_TAM);
  
  /*
    //menu_set_index_menu(&sens_menu_quant_sensores, 1);
    printf("%d \n", sens_menu_quant_sensores.index_atual);
    //printf("%d \n", sens_menu_quant_sensores.valor_atual);
    printf("%d \n", sens_menu_quant_sensores.quant_itens);
    printf("%s \n", sens_menu_quant_sensores.itens[0].str_valor);
    printf("%s \n", sens_menu_itens_quant_sensores[0].str_valor);
  */
  
  lcd_goto(2, 0);
  lcd_puts("Pronto!    ");
    
  btns_init();

  INTCONbits.GIE = 1; //habilita interrupcoes globais    
            
  while (1) {
    #ifdef _MODULO_ANTIGO_
      //11 niveis de stack
      option = btns_testa();
      if (option != 0) {
        //btns_processa(option);
        est_maquina(option);
      }
    #endif
  }//while (1))
  return (EXIT_SUCCESS);
}//main()
