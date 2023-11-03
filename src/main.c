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
#include "versao.h"
#include "xtal.h"
#include "adcon.h"
#include "botoes.h"
#include "lcd.h"
#include "rs232.h"
#include "timer.h"
#include "cfg_quant_sensores.h"
#include "cfg_tempo_amostra.h"
#include "menu_principal.h"
#include "servicos.h"

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
 * Funcao principal: inicializa os dispositivos do PIC, inicializa menus, 
 * configura a interrupcao global, etc...
 * e fica em while().
 * @return EXIT_SUCCESS
 */
int main(void) {
  
  //rs232_init() precisa ser antes do printf() e antes do lcd_puts() pois 
  //em modo __DEUBG essas funções utilizam a UART.
  rs232_init();
  lcd_init();
  
  //char teste[10];
  //sprintf(teste, "teste");
  //char teste2;
  //perror(&teste2);
  //fprintf(stderr, "teste\n");

  lcd_puts("2023-11-02 - v1.61-");
  #ifdef _MODULO_NOVO_
    lcd_puts("N");
  #endif
  #ifdef _MODULO_ANTIGO_
    lcd_puts("A");
  #endif
  //Antes escrevia D (de DEBUG), mas esta diretiva foi modificada em 2023-08-21.
  #ifdef _ENVIA_DADOS_SERIAL_
    lcd_puts("S");
  #endif

  lcd_goto(2, 0);
  lcd_puts("Iniciando...");

  adcon_init();
  timer0_init();
  btns_init();

  TXSTAbits.TXEN = 1; // enable transmitter
  RCSTAbits.SPEN = 1; 
  
  // the Power-up Timer (72 ms duration) prevents EEPROM write:
  // 140ms = tempo maximo do power-up:
  __delay_ms(200);

  //Testa se a EEPROM já foi gravada pelo equipamento.
  //Se ainda não foi, então inicializa com os valores de configuração padrão.
  if (!serv_eeprom_verifica_inicializacao()) {
    serv_eeprom_inicializa_configuracoes();
    serv_eeprom_limpa_dados();
  }
  else {
    //Chave de inicialização existe, então le as configurações da EEPROM.
    serv_eeprom_le_configuracoes();
  }
  
  //Inicializa os menus:
  menu_init(&menu_principal, menu_principal_itens, MENU_PRINCIPAL_TAM);
  menu_init(&menu_cfg_quant_sensores, menu_cfg_quant_sensores_itens, MENU_CFG_QUANT_SENSORES_TAM);
  menu_init(&menu_cfg_tempo_amostra, menu_cfg_tempo_amostra_itens, MENU_CFG_TEMPO_AMOSTRA_TAM);
  
  menu_set_value_indexes(&menu_cfg_quant_sensores, cfg_quant_sensores_atual);
  menu_set_value_indexes(&menu_cfg_tempo_amostra,  cfg_tempo_amostra_atual);
  
  lcd_goto(2, 0);
  lcd_puts("Pronto!    ");

  INTCONbits.GIE = 1; //habilita interrupcoes globais    
            
  while (1) {
    #ifdef _MODULO_ANTIGO_
      TBotao option;
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
