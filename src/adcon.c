#include <stdint.h>
#include <stdlib.h>
#include <xc.h>
#include <stdio.h>

#include "adcon.h"
#include "versao.h"
#include "eeprom.h"
#include "lcd.h"
#include "cfg_quant_sensores.h"
#include "xtal.h"

#ifdef _MQ_
#include "mq6.h"
#endif

//Quantidade de aquisições a serem feitas em um sensor para calcular a média 
//e gerar o valor da leitura do sensor.
#define ADCON_QUANT_AQUISICOES_MEDIA_LEITURA 32

/*
Configuração dos canais do conversor analógico/digital:
Canal - Pino - Sensor
0000  - AN0  - Sensor 1
0001  - AN1  - Sensor 2
0010  - AN2  - Sensor 3
0011  - AN3
0100  - AN4  - Sensor 4
0101  - AN5
0110  - AN6
0111  - AN7
1000  - AN8
1001  - AN9
1010  - AN10
1011  - AN11
1100  - AN12
1101  - AN13
1110  - CV REF
1111  - Fixed Ref (0.6 volt fixed reference)
*/

//Canais em que estão cada um dos 4 sensores:
static const uint8_t canais[CFG_QUANT_SENSORES_MAX] = {
    0,
    1,
    2,
    4
};

/*const uint8_t canais[CFG_QUANT_SENSORES_MAX] = {
    0,
    1,
    2,
    4
};
 */
/*versao 8 sensores
const uint8_t canais[TAM_MENU_QUANT_SENSORES] = {
    0,
    1,
    2,
    4,  //ra5
    12, //rb0
    10, //rb1
    8,  //rb2
    9   //rb3
};
*/

uint16_t adcon_leitura_min = 0;
uint16_t adcon_leitura_max = 0;
uint8_t  adcon_quant_amostras_gravadas = 0; //Total de amostras gravadas na EEPROM.

static uint16_t adcon_le_sensor(uint8_t num_sensor);

/**
 * Funcao que inicializa o conversor analogico/digital. e chamada na funcao main().
 */
void adcon_init(void) {
  TRISA = 0xff;
  #ifdef _PIC16F886_H_
    ANSEL = 0b00011111; /// configura porta como analogica
    //(AN7:5) nao implementadas no PIC16F886
    ADCON1bits.VCFG1 = 0; // Referencia negativo
    // 1 = pino Vref- ; 0 = Vss
    ADCON1bits.VCFG0 = 1; // Referencia positivo
    // 1 = pino Vref+ ; 0 = Vdd
  #endif
  #ifdef _PIC16F876A_H_
    ADCON1bits.PCFG = 0b0001; //< configurado para q o pino seja a referencia de tensao
  #endif
  ADCON0bits.ADCS = 1; // frequencia de conversao: FOSC/8 (recomendado para 4MHz)
  ADCON1bits.ADFM = 1; // resultado justificado a direita
  ADCON0bits.ADON = 1; //< liga conversor A/D
}//adcon_init()

/**
 * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem, ou seja, le os valores dos sensores
 * e imprime no display.
 */
void adcon_amostra_print(void) {
  lcd_clear();
  
  for (uint8_t i = 0; i < cfg_quant_sensores_atual; i++) {
    uint16_t x = adcon_le_sensor(i);
    #ifdef _LM35_
      adcon_print(x, i);
    #endif
    #ifdef _MQ_
      mq_mostra(x, i);
    #endif
  }//for
}//adcon_amostra_print()

/**
 * * Funcao chamada a partir da contagem do timer.
 * Faz uma amostragem, ou seja, le os valores dos sensores,
 * imprime no display e grava na EEPROM.
 * Pode atualizar e gravar também os valores de leitura MAX e MIN. 
*/
void adcon_amostra_print_grava(void) {
  uint16_t t_int;
  uint16_t maior, menor;
  uint8_t qtd_leituras;

   qtd_leituras = adcon_quant_amostras_gravadas * cfg_quant_sensores_atual;
  
  maior = eeprom_le_word(EEPROM_END_LEITURA_MAX);
  menor = eeprom_le_word(EEPROM_END_LEITURA_MIN);

  //<<<<<<<<<<<<<<
  //testar aqui se vai dar para gravar todos os valores

  lcd_clear();
  if (qtd_leituras < ADCON_QTD_MAX_LEITURAS) {
    for (uint8_t i = 0; i < cfg_quant_sensores_atual; i++) {
      t_int = adcon_le_sensor(i);
      if (t_int < menor) {
        eeprom_grava_word(EEPROM_END_LEITURA_MIN, t_int);
      }
      if (t_int > maior) {
        eeprom_grava_word(EEPROM_END_LEITURA_MAX, t_int);
      }
      eeprom_grava_word(EEPROM_END_INICIO_AMOSTRAS + (qtd_leituras * 2), t_int);
      qtd_leituras++;
      adcon_print(t_int, i);
    } // for (uint8_t i = 0; i < qtd_sens; i++)
    adcon_quant_amostras_gravadas++;
    eeprom_write(EEPROM_END_QTDE_AMOSTRAS, adcon_quant_amostras_gravadas);
  }//if (qtd_leituras < ADCON_QTD_MAX_LEITURAS) {
  else {
    //Desabilita a interrupção do Timer0 para parar as leituras.
    INTCONbits.T0IE = 0;
    lcd_puts("Memoria Cheia");
  } //else
} //adcon_amostra_print_grava()


/**
 * Funcao que divide o valor_sensor por 10 e mostra no display o quociente e o resto, 
 * separando-os por um ponto para indicar o separador de decimal.
 * @param valor_sensor valor valor a ser mostrado no display.
 * @param num_sensor número do sensor (0, 1, 2, 3) que está sendo mostrado. 
 * A partir desse dado a função saberá em qual posição do display deverá mostrar o valor_sensor.
 */
void adcon_print(uint16_t valor_sensor, uint8_t num_sensor) {
  //Desta maneira deu problemas no display:
  //char temp_str2[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
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
}//adcon_print()

/**
 * Funcao que faz a leitura de um sensor.
 * Essa leitura consiste em efetuar várias aquisições na porta analógica,
 * conforme indicado pela constante ADCON_QUANT_AQUISICOES_MEDIA_LEITURA e
 * efetuar a média dos valores dessas aquisições, resultando no valor
 * da leitura do sensor.
 * @param num_sensor número do sensor a ser lido (0 a 3).
 * @return valor varlor lido do sensor (média das aquisições).
 */
static uint16_t adcon_le_sensor(uint8_t num_sensor) {
    /*
    ADCON0bits.CHS = canais[cnl]; // selecao de canal
    //tempo necessario (pior caso) apos troca de canal:
    __delay_us(20);
    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE); // espera terminar conversao

    return (ADRESH << 8) +ADRESL;
     */
    //const int n_valores = 30;

    //uint16_t acc = 0;
    uint32_t acc = 0;
    ADCON0bits.CHS = canais[num_sensor]; // selecao de canal
    __delay_us(20);
    //tempo necessario (pior caso) apos troca de canal
    for (uint16_t i = 0; i < ADCON_QUANT_AQUISICOES_MEDIA_LEITURA; i++) {

        ADCON0bits.GO_nDONE = 1;
        while (ADCON0bits.GO_nDONE); // espera terminar conversao
        acc += (uint32_t)((ADRESH << 8) + ADRESL); //10bits
    }//for

    //acc (11 bits) para 10:
    return (uint16_t)(acc / ADCON_QUANT_AQUISICOES_MEDIA_LEITURA);
    //return (ADRESH << 8) +ADRESL;
}//adcon_ler_sensor()
