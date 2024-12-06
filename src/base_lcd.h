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

//===== Includes =============================================================
 #include "versao.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================

#if defined(_HARDWARE_2013_)
    #define LCD_PIN_RS              RB0
    #define LCD_PIN_E               RB1
    #define LCD_PIN_D4              RB2
    #define LCD_PIN_D5              RB3
    #define LCD_PIN_D6              RB4
    #define LCD_PIN_D7              RB5
#elif defined(_HARDWARE_2016_)
    #define LCD_PIN_RS              RC0
    #define LCD_PIN_E               RC1
    #define LCD_PIN_D4              RC3 //D4 e D5 trocados devido � posi��o j� existente na vers�o 2016 do hardware.
    #define LCD_PIN_D5              RC2 //D4 e D5 trocados devido � posi��o j� existente na vers�o 2016 do hardware.
    #define LCD_PIN_D6              RC5 //D6 e D7 trocados devido � posi��o j� existente na vers�o 2016 do hardware.
    #define LCD_PIN_D7              RC4 //D6 e D7 trocados devido � posi��o j� existente na vers�o 2016 do hardware.
#endif
      
//===== Tipos P�blicos =======================================================

//===== Variaveis P�blicas ===================================================

//===== Funcoes P�blicas =====================================================
      
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
     * Muda a posi��o do curso no display para a posi��o onde deve ser impresso 
     * o sensor de n�mero num_sensor.
     */
    extern void lcd_goto_sensor(uint8_t num_sensor);

    /**
    * Envia uma string para o LCD. Se estiver em modo __DEBUG, envia tamb�m para a UART.
    * @param *str string a ser enviada.
    */
    void lcd_puts(const char *str);


  #ifdef	__cplusplus
  }
  #endif

#endif /* BASE_LCD_H */