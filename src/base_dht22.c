/*****************************************************************************
 * File:   base_dht22.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 *
 ****************************************************************************/

//===== Includes =============================================================
//#include <xc.h>
#include "base_dht22.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Vari�veis P�blicas ===================================================
//DHT22 transmite 40 bits
//8 bits parte inteira da umidade.
//8 bits parte decimal da umidade.
//8 bits parte inteira da temperatura.
//8 bits parte decimal da temperatura.
//8 bits de checksum.
uint16_t base_dht22_amostra_umidade;
uint16_t base_dht22_amostra_temperatura;

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Vari�veis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================
static void base_dht22_start(void);
static char base_dht22_verifica_resposta(void);
static char base_dht22_le_byte(unsigned char * dht_data);

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

/****************************************************************************
* Faz uma amostra e armazena nas vari�veis base_dht22_amostra_umidade e 
* base_dht22_amostra_temperatura.
 ***************************************************************************/
char base_dht22_amostra(void) {
  unsigned char low_byte;
  unsigned char high_byte;
  unsigned char sum = 0;
  
  base_dht22_start();
  
  if (!base_dht22_verifica_resposta())
  return 0;
  
  if (!base_dht22_le_byte(&high_byte))
    return 0;
  sum += high_byte;
  
  if (!base_dht22_le_byte(&low_byte))
    return 0;
  sum += low_byte;
  
  base_dht22_amostra_umidade = (high_byte<<8) + low_byte;
  
  if (!base_dht22_le_byte(&high_byte))
    return 0;
  sum += high_byte;
  
  if (!base_dht22_le_byte(&low_byte))
    return 0;
  sum += low_byte;
  
  base_dht22_amostra_temperatura = (high_byte<<8) + low_byte;

  //Le checksum.
  if (!base_dht22_le_byte(&low_byte))
    return 0;
  
  //Conferir checksum
  if (sum == low_byte)
    return 1;
  else
    return 0;

}//base_dht22_amostra()

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

/****************************************************************************
* Envia os sinais para solicitar a leitura dos dados do DHT22.
***************************************************************************/
static void base_dht22_start(void){
  BASE_DHT22_TRIS = 0;     // configure DHT22_PIN como sa�da.
  BASE_DHT22_PIN  = 0;     // coloca zero em DHT22_PIN.
 
  __delay_ms(25);          // esper 25ms (milissegundos, datasheet diz pelo menos 18ms).
  
  //N�o precisa pois a porta � pull-up e automaticamente colocar� em 1
  //quando for configurada como entrada.
  //BASE_DHT22_PIN = 1;      // set DHT22_PIN output (logic 1)
  //__delay_us(30);          // wait 30 us
  
  BASE_DHT22_TRIS = 1;     // configure DHT22_PIN como entrada
  __delay_us(30);          // espera 30 us (microssegundos).
}//base_dht22_start())

/****************************************************************************
* L� a porta para verificar se o DHT22 respondeu ao comando de leitura.
* Demora em torno de 160us.
* Desabilitar todas interrup��es antes de chamar esta fun��o.
* Retorna 1 se ok ou zero se erro.
*****************************************************************************/
static char base_dht22_verifica_resposta(void){

  TMR1H  = 0;                 // reset Timer1
  TMR1L  = 0;
  
  //Liga o Timer1.
  //TMR1ON         = 1;   
  //TMR1ON_bit     = 1;
  T1CONbits.TMR1ON = 1;
 
  //Teste de 80us em n�vel baixo.
  while(!BASE_DHT22_PIN && TMR1L < 100) {
    //Espera at� DHT22_PIN ficar em 1.
    //Usando timeout de 99us.
  };  
  //if response time > 99�S  ==> Response error
  if(TMR1L > 99) { 
    T1CONbits.TMR1ON = 0;
    return 0; // return 0 (Device has a problem with response)
  }

  //Reseta Timer1.
  TMR1H = 0;     
  TMR1L = 0;
 
  //Teste de 80us em n�vel alto.
  while(BASE_DHT22_PIN && TMR1L < 100) {
    //Espera at� DHT22_PIN ficar em 0.
    //Usando timeout de 99us.
  }
  // if response time > 99�S  ==> Response error
  if(TMR1L > 99) {
    T1CONbits.TMR1ON = 0;
    return 0; // return 0 (Device has a problem with response)
  }
  
  T1CONbits.TMR1ON = 0;
  return 1; //Tudo ok.

}//base_dht22_verifica_resposta())

/****************************************************************************
* L� 8 bits da porta e coloca no argumento dht_data.
* Retorna 1 se ok ou zero se erro.
*****************************************************************************/
static char base_dht22_le_byte(unsigned char * dht_data) { 
  *dht_data = 0;
 
  for(char i = 0; i < 8; i++) {
    //Reseta Timer1.
    TMR1H = 0;  
    TMR1L = 0;
    
    //Liga Timer1
    T1CONbits.TMR1ON = 1;
    
    //Teste de n�vel baixo (timeout de 99us, normalmente demora 50us).
    while(!BASE_DHT22_PIN && TMR1L < 100) {  
      if(TMR1L > 99) { //Excedeu 99us (erro timeout).
        //Desliga Timer1
        T1CONbits.TMR1ON = 0;
        return 0;
      }
    }

    //Resteta Timer1.
    TMR1H = 0;       
    TMR1L = 0;
 
    //Teste de n�vel alto (timeout de 99us).
    while(BASE_DHT22_PIN && TMR1L < 100) {      
      if(TMR1L > 99) { //Excedeu 99us (erro timeout).
        //Desliga Timer1
        T1CONbits.TMR1ON = 0;
        return 0;       
      }
    }
 
    //Verifica a dura��o do pulso alto:
    //26us a 28us significa n�vel 0.
    //70us        significa n�vel 1.
     if(TMR1L > 50) {               
       *dht_data |= (1 << (7 - i));  // set bit (7 - i)
     }
  }//for
 
  //Desliga Timer1
  T1CONbits.TMR1ON = 0;

  //Byte lido com sucesso.
  return 1;
  
}//base_dht22_le_byte()

//===== Final do Arquivo =====================================================
