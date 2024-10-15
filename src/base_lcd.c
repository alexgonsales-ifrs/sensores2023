/*****************************************************************************
 * File:   base_lcd.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================

#include <stdint.h>
#include <xc.h>

#include "base_lcd.h"
#include "versao.h"
#include "xtal.h"   //usado por __delay_us())

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

  
//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

typedef struct {
    uint8_t linha;
    uint8_t coluna;
  } TLCDPosicao;
  
  //Esse vetor contém as posições (linha e coluna) 
  //onde inicia a impressão de cada sensor no LCD.
  //Linhas: 1 e 2.
  //Colunas: 0 a 15.
#if defined (_HARDWARE_2013_)
  const TLCDPosicao LCD_V_POSICOES[] = {
    1, 0, //Sensor 1: linha 1 x coluna 0
    1, 8, //Sensor 2: linha 1 x coluna 8
    2, 0, //Sensor 3: linha 2 x coluna 0
    2, 8  //Sensor 4: linha 2 x coluna 8. Não colocar vírgula no último.
  };
#elif defined (_HARDWARE_2016_)
  const TLCDPosicao LCD_V_POSICOES[] = {
    1, 0,  //Sensor 1: linha 1 x coluna 0
    1, 4,  //Sensor 2: linha 1 x coluna 4
    1, 8,  //Sensor 3: linha 1 x coluna 8
    1, 12, //Sensor 4: linha 1 x coluna 12
    2, 0,  //Sensor 5: linha 2 x coluna 0
    2, 4,  //Sensor 6: linha 2 x coluna 4
    2, 8,  //Sensor 7: linha 2 x coluna 8
    2, 12  //Sensor 8: linha 2 x coluna 12. Não colocar vírgula no último.
  };
#endif

//===== Variaveis Privadas ===================================================
  
//===== Declaração das Funções Privadas ======================================
  
static void lcd_write(uint8_t c);

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

/**
* Rotina para inicializar o display. É chamada na funcao main().
*/
void lcd_init(void) {
    LCD_RS = LCD_E = LCD_D4 = LCD_D5 = LCD_D6 = LCD_D7 = 0; // display, 0 = saida
    lcd_write(2);
    lcd_write(8);
    lcd_write(0);
    lcd_write(0);
    lcd_write(0x0C);
    lcd_write(0);
    lcd_write(6);
}//lcd_init())

/**
* Limpa o display e aguarda 2ms.
*/
void lcd_clear(void) {
    LCD_RS = 0;
    lcd_write(0);
    lcd_write(1);
    __delay_ms(2);
}//lcd_clear())

 /**
 * Muda a posicao do cursor no display.
 * @param linha numero da linha (1 ou 2)
 * @param coluna numero da coluna (0 a 15)
 * 
 * Comandos que devem ser enviados ao LCD para endereçar (posicionar o cursor)
 * em cada uma das posições de um display 2 (linhas) x 16 (colunas)
 * 80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F
 * C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF
 
 * que equivalem aos seguintes endereços da DDRAM do display:
 * 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
 * 40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
 * 
 * Exemplo: para posicionar o cursor na linha 1 e coluna 0, 
 * deve ser enviado o comando 0x80.
 */
void lcd_goto(uint8_t linha, uint8_t coluna) {
  LCD_RS = 0;
  switch (linha) {
    case 1:
      lcd_write(0x08);
      break;
    case 2:
      lcd_write(0x0C);
    break;
  }
  lcd_write(coluna);
  __delay_us(40);
}//lcd_goto())

/*
* Muda a posição do curso no display para a posição onde deve ser impresso 
* o sensor de número num_sensor.
*/
void lcd_goto_sensor(uint8_t num_sensor) {
  //2023-11-26: modificado para diminuir a call stack.
  //Mudança: RAM=312 Program=7388 para RAM=312 Program=7430.
  
  //2023-11-26: retirado este.
  //lcd_goto(LCD_V_POSICOES[num_sensor].linha, LCD_V_POSICOES[num_sensor].coluna);

  //2023-11-26: adicionado isto.
  uint8_t linha, coluna;
  linha  = LCD_V_POSICOES[num_sensor].linha;
  coluna = LCD_V_POSICOES[num_sensor].coluna;
  LCD_RS = 0;
  switch (linha) {
    case 1:
      lcd_write(0x08);
      break;
    case 2:
      lcd_write(0x0C);
    break;
  }
  lcd_write(coluna);
  __delay_us(40);
  
  
}//lcd_goto_sensor())

/**
 * Envia uma string para o LCD.
 * Se o MPLABx IDE estiver em modo __DEBUG então envia a string também para a UART,
 * para imprimir na janela "UART1 Output" da IDE e permitir se visualizar durante o DEBUG.
 * Nesse caso, ao enviar para a UART, coloca o prefixo "LCD:" de modo a poder diferenciar
 * se o conteúdo enviado para a UART é um conteúdo "legítimo" ou apenas DEBUG do LCD.
 * @param *str string a ser enviada.
 */
void lcd_puts(const char *str) {
  uint8_t cmd; //comando a ser enviado ao LCD.
  LCD_RS = 1;
  char c;
  const char *p_char;

  //Se estiver no modo __DEBUG então coloca o prefixo "LCD:" antes de enviar para a UART.
  #ifdef __DEBUG
    const char *p_char_lcd_debug_serial = "LCD:"; 
    p_char = p_char_lcd_debug_serial;
    while(*p_char) {
      while (!TXIF) {
        continue;
      }
      TXREG = *p_char;
      p_char++;
    }
  #endif

  p_char = str;

  while (*p_char) {
    //Desloca 4 bits para a direita para enviar os 4 bits mais significativos do caractere.
    c = *p_char;
    cmd = c >> 4;
    lcd_write(cmd);
    __delay_us(40);
        
    //Envia os 4 bits menos significativos do caractere.
    lcd_write(*p_char);
    __delay_us(40);

    //Se o MPLABx IDE estiver em modo __DEBUG então envia o print do LCD também para a UART,
    //para imprimir na janela "UART1 Output" da IDE.
    #ifdef __DEBUG
      #ifdef _DEBUG_LCD_CMD_
        //Desloca 4 bits para a direita para enviar os 4 bits mais significativos do caractere.
        cmd = *str >> 4;
        while (!TXIF) {
          continue;
        }
        TXREG = cmd;
        
        while (!TXIF) {
          continue;
        }      
        //Envia os 4 bits menos significativos do caractere.
        cmd = (*str) & 0b00001111;
        TXREG = cmd;
      #else
        cmd = *p_char;
        //while (! PIR1bits.TXIF) {
        while (!TXIF) {
          continue;
        }
        TXREG = cmd;
      #endif
    #endif      

    p_char++;
        
  }//while
  
  //Se estiver em modo __DEBUG então envia um comando para troca de linha.
  #ifdef __DEBUG
    while (!TXIF) {
      continue;
    }      
    TXREG = '\n';
  #endif

}//lcd_puts()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

/**
 * Envia o comando para o display separando cada bit
 * @param c comando a ser enviado
 */
static void lcd_write(uint8_t c) {
  if (c & 0b00000001) {
    LCD_D4 = 1;
  } else {
    LCD_D4 = 0;
  }
  if (c & 0b00000010) {
    LCD_D5 = 1;
  } else {
    LCD_D5 = 0;
  }
  if (c & 0b00000100) {
    LCD_D6 = 1;
  } else {
    LCD_D6 = 0;
  }
  if (c & 0b00001000) {
    LCD_D7 = 1;
  } else {
    LCD_D7 = 0;
  }
  __delay_us(40);
  LCD_E = 1;
  LCD_E = 0;
}//lcd_write())

//===== Final do Arquivo =====================================================