/* 
 * File:   versao.h
 * Author: ifrs
 *
 * Created on 5 de Maio de 2014, 16:00
 * 
 * ATEN��O:
 * 
 * Nota1: este arquivo s� pode conter defini��es de macros, pois ele foi incluido
 * no arquivo main.c que cont�m as defini��es PRAGMA e estas s� podem ser utilizadas
 * antes da inclus�o de arquivos headers. Veja Nota1 em arquivo main.c.
 */

#ifndef VERSAO_H
#define	VERSAO_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
//N�o deveria existir sob risco de infrigir a Nota1.

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//Esta diretiva ser� configurada pela IDE quando configuramos o modelo de PIC nas op��es de projeto.
//No entanto, � interessante defin�-la aqui tamb�m (de forma correta) para que o compilador reconhe�a as vari�veis definidas nesse modelo de PIC.
//#define _PIC16F886_H_

//As diretivas a seguir definem a placa de hardware para o qual o software 
//ser� compilado. Apenas uma delas pode estar definida.
    
//#define _HARDWARE_2013_ //PIC16F876A (n�o esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).
#define _HARDWARE_2016_ //PIC16F886  (n�o esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).

//Define uma string para a vers�o do software.
//Formato: AAAAMMDDvX.YY
//AAAA = Ano
//MM   = Mes
//DD   = Dia
//X.YY = Numera��o (atualmente X=1 e Y=700)
#define _VER_STR_VERSAO_ "20250822v1.726"
  
#ifdef _HARDWARE_2013_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "A"
#endif 
#ifdef _HARDWARE_2016_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "N"
#endif

//Esta diretiva � definida pela IDE quando o projeto � executado em modo DEBUG.
//� interessante tamb�m habilit�-la aqui quando o projeto for executado em modo DEBUG
//pois assim a IDE mostra o c�digo que ser� compilado.
//#define __DEBUG
    
//Esta diretiva somente ser� usada quando se desejar efetuar o DEBUG do LCD.
//Nessa caso, os comandos de escrita no LCD ser�o enviados para a porta serial 
//por nibble (4 bits) ao inv�s do byte inteiro.
//#define _DEBUG_LCD_CMD_

//===== Constantes P�blicas ==================================================
//N�o deveria existir sob risco de infrigir a Nota1.
    
//===== Tipos P�blicos =======================================================
//N�o deveria existir sob risco de infrigir a Nota1.
    
//===== Variaveis P�blicas ===================================================
//N�o deveria existir sob risco de infrigir a Nota1.
    
//===== Funcoes P�blicas =====================================================
//N�o deveria existir sob risco de infrigir a Nota1.
    
//============================================================================
    
#ifdef	__cplusplus
}
#endif

#endif	/* VERSAO_H */
