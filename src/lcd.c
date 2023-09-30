#include <stdint.h>
#include <xc.h>

#include "lcd.h"
#include "versao.h"
#include "xtal.h" //usado por __delay_us())

    typedef struct {
        uint8_t linha;
        uint8_t coluna;
    } TLCDPosicao;
    
    const TLCDPosicao LCD_V_POSICOES[] = {
        1, 0,
        1, 8,
        2, 0,
        2, 8
    };

    /*versao 8 sensores
    const S_pos LCD_POSICAO[TAM_MENU_QUANT_SENSORES] = {
        1, 0,
        1, 8,
        2, 0,
        2, 8,
        1, 0,
        1, 8,
        2, 0,
        2, 8
    };
     */

static void lcd_write(uint8_t c);

/**
* Rotina para inicializar o display. E chamada na funcao main().
*/
void lcd_init(void) {
    LCD_RS_TRIS = LCD_E_TRIS = LCD_D4_TRIS = LCD_D5_TRIS = LCD_D6_TRIS = LCD_D7_TRIS = 0;
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
 * 
 * que equivalem aos seguintes endereços da DDRAM do display:
 * 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
 * 40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
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
  lcd_goto(LCD_V_POSICOES[num_sensor].linha, LCD_V_POSICOES[num_sensor].coluna);
}

/**
 * Envia uma string para o LCD. Se estiver em modo __DEBUG, envia também para a UART.
 * @param *str string a ser enviada.
 */
void lcd_puts(const char *str) {
  uint8_t cmd; //comando a ser enviado ao LCD.
  LCD_RS = 1;

  while (*str) {
    //Desloca 4 bits para a direita para enviar os 4 bits mais significativos do caractere.
    cmd = *str >> 4;
    lcd_write(cmd);
    __delay_us(40);
        
    //Envia os 4 bits menos sigificativos do caractere.
    lcd_write(*str);
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
        cmd = *str;
        while (!TXIF) {
          continue;
        }
        TXREG = cmd;
        
      #endif
    #endif      

    str++;
        
  }//while
  
  //Se estiver em modo __DEBUG então envia um comando para troca de linha.
  #ifdef __DEBUG
    while (!TXIF) {
      continue;
    }      
    TXREG = '\n';
  #endif

}//lcd_puts()

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
