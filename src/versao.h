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
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//Esta diretiva ser� configurada pela IDE quando configuramos o modelo de PIC nas op��es de projeto.
//No entanto, � interessante defin�-la aqui tamb�m (de forma correta) para que o compilador reconhe�a as vari�veis definidas nesse modelo de PIC.
//#define _PIC16F886_H_

//As duas diretivas a seguir definem se o software ser� compilado para a
//vers�o antiga do hardware ou vers�o nova do hardware.
//Apenas uma das duas pode ser definida.
    
//#define _HARDWARE_NOVO_ //PIC16F886  (n�o esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).
#define _HARDWARE_ANTIGO_ //PIC16F876A (n�o esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).

//Define a vers�o do software AAAAMMDDvX.YY
//AAAA = Ano
//MM   = Mes
//DD   = Dia
//X.YY = Numera��o (atualmente X=1 e Y=700)

 //===== Constantes P�blicas ==================================================
    
#define _VER_STR_VERSAO_ "20240920v1.701"
  
#ifdef _HARDWARE_NOVO_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "N"
#endif
#ifdef _HARDWARE_ANTIGO_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "A"
#endif 

//===== Tipos P�blicos =======================================================
    
//===== Variaveis P�blicas ===================================================
    
//Esta diretiva � definida pela IDE quando o projeto � executado em modo DEBUG.
//� interessante tamb�m habilit�-la aqui quando o projeto for executado em modo DEBUG,
//pois assim a IDE mostra o c�digo que ser� compilado.
#define __DEBUG
    
//Esta diretiva somente ser� usada quando o projeto estiver em modo __DEBUG.
//Nessa caso, se esta diretiva estiver ativada, ent�o os comandos de escrita no LCD
//ser�o enviadas por nibble (4 bits) ao inv�s do byte inteiro.
//#define _DEBUG_LCD_CMD_

//#define _MQ_
#define _LM35_

//===== Funcoes P�blicas =====================================================

//============================================================================
    
#ifdef	__cplusplus
}
#endif

#endif	/* VERSAO_H */
