/* 
 * File:   main.c
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 15:57
 */
#ifdef _MODULO_NOVO_
//  #pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

#ifdef _MODULO_ANTIGO_
  //#pragma config CP = OFF, BOREN = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT, LVP = OFF
#endif

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

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
#include "menu_principal.h"
#include "serv_eeprom.h"
#include "estados.h"

//Esta função é usada somente para marcação no Clock Stimulus
//void inicializado3(void){ };

#ifdef __DEBUG
//Esta macro é implementada somente em modo __DEBUG 
//para que a saída do printf() seja na UART do simulador.
void putch(char dado) {
  while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
    continue;
  TXREG = dado;                     // send one character
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

  lcd_puts("20231128v1.65");
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

  //Alexdg: retirado em 2023-11-03, pois já está no rs232_init().)
  //TXSTAbits.TXEN = 1; // enable transmitter
  //RCSTAbits.SPEN = 1; 
  
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
  
  menu_set_value_indexes(&menu_cfg_quant_sensores, adcon_cfg_quant_sensores_atual);
  menu_set_value_indexes(&menu_cfg_tempo_amostra,  adcon_cfg_tempo_amostra_atual);
  
  lcd_goto(2, 0);
  lcd_puts("Pronto!         ");

  INTCONbits.GIE = 1; //habilita interrupcoes globais.
            
  while (1) {
    #ifdef _MODULO_ANTIGO_
      TBotao option;
      //Este microcontrolador tem 8 níveis de stack mas este código pode alcançar até 12 niveis.
      //INTCONbits.GIE = 0; //Desabilita interrupcoes globais para não causar stack "overflow".
      option = btns_testa_antigo();
      if (option != 0) {
        est_maquina(option);
      }
      //INTCONbits.GIE = 1; //Habilita interrupcoes globais.
      __delay_us(20);
    #endif
  }//while (1))
  return (EXIT_SUCCESS);
}//main()
