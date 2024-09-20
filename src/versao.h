/* 
 * File:   versao.h
 * Author: ifrs
 *
 * Created on 5 de Maio de 2014, 16:00
 */

#ifndef VERSAO_H
#define	VERSAO_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//Esta diretiva será configurada pela IDE quando configuramos o modelo de PIC nas opções de projeto.
//No entanto, é interessante definí-la aqui também (de forma correta) para que o compilador reconheça as variáveis definidas nesse modelo de PIC.
//#define _PIC16F886_H_

//As duas diretivas a seguir definem se o software será compilado para a
//versão antiga do hardware ou versão nova do hardware.
//Apenas uma das duas pode ser definida.
    
//#define _HARDWARE_NOVO_ //PIC16F886  (não esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).
#define _HARDWARE_ANTIGO_ //PIC16F876A (não esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).

//Define a versão do software AAAAMMDDvX.YY
//AAAA = Ano
//MM   = Mes
//DD   = Dia
//X.YY = Numeração (atualmente X=1 e Y=700)

 //===== Constantes Públicas ==================================================
    
#define _VER_STR_VERSAO_ "20240920v1.701"
  
#ifdef _HARDWARE_NOVO_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "N"
#endif
#ifdef _HARDWARE_ANTIGO_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "A"
#endif 

//===== Tipos Públicos =======================================================
    
//===== Variaveis Públicas ===================================================
    
//Esta diretiva é definida pela IDE quando o projeto é executado em modo DEBUG.
//É interessante também habilitá-la aqui quando o projeto for executado em modo DEBUG,
//pois assim a IDE mostra o código que será compilado.
#define __DEBUG
    
//Esta diretiva somente será usada quando o projeto estiver em modo __DEBUG.
//Nessa caso, se esta diretiva estiver ativada, então os comandos de escrita no LCD
//serão enviadas por nibble (4 bits) ao invés do byte inteiro.
//#define _DEBUG_LCD_CMD_

//#define _MQ_
#define _LM35_

//===== Funcoes Públicas =====================================================

//============================================================================
    
#ifdef	__cplusplus
}
#endif

#endif	/* VERSAO_H */
