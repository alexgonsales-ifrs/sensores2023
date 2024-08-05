/* 
 * File:   lcd.h
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 17:18
 */
 #ifndef BASE_LCD_H
 #define BASE_LCD_H

  #ifdef	__cplusplus
  extern "C" {
  #endif

 #include "versao.h"

#if defined(_HARDWARE_NOVO_)
    #define LCD_RS              RC0
    #define LCD_RS_TRIS         TRISC0
    #define LCD_E               RC1
    #define LCD_E_TRIS          TRISC1
    #define LCD_D4              RC2
    #define LCD_D4_TRIS         TRISC2

    #define LCD_D5              RB2
    #define LCD_D5_TRIS         TRISB2
    #define LCD_D6              RB1
    #define LCD_D6_TRIS         TRISB1
    #define LCD_D7              RB0
    #define LCD_D7_TRIS         TRISB0
    /*
    #define LCD_D5              RC3
    #define LCD_D5_TRIS         TRISC3
    #define LCD_D6              RC4
    #define LCD_D6_TRIS         TRISC4
    #define LCD_D7              RC5
    #define LCD_D7_TRIS         TRISC5
     */
#elif defined(_HARDWARE_ANTIGO_)
      #define LCD_RS              RB0
      #define LCD_RS_TRIS         TRISB0
      #define LCD_E               RB1
      #define LCD_E_TRIS          TRISB1
      #define LCD_D4              RB2
      #define LCD_D4_TRIS         TRISB2
      #define LCD_D5              RB3
      #define LCD_D5_TRIS         TRISB3
      #define LCD_D6              RB4
      #define LCD_D6_TRIS         TRISB4
      #define LCD_D7              RB5
      #define LCD_D7_TRIS         TRISB5
#endif

    /**
     * Rotina para inicializar o display. E chamada na funcao main().
     */
    extern void lcd_init(void);

    /**
     * Limpa o display e aguarda 2ms.
     */
    extern void lcd_clear(void);

    /**
     * Muda a posicao do cursor no display.
     * @param linha numero da linha (1 ou 2)
     * @param coluna numero da coluna (0 a 15)
     */
    extern void lcd_goto(uint8_t linha, uint8_t coluna);

    /*
     * Muda a posição do curso no display para a posição onde deve ser impresso 
     * o sensor de número num_sensor.
     */
    extern void lcd_goto_sensor(uint8_t num_sensor);

    /**
    * Envia uma string para o LCD. Se estiver em modo __DEBUG, envia também para a UART.
    * @param *str string a ser enviada.
    */
    void lcd_puts(const char *str);


  #ifdef	__cplusplus
  }
  #endif

#endif /* BASE_LCD_H */

