/* 
 * File:   versao.h
 * Author: ifrs
 *
 * Created on 5 de Maio de 2014, 16:00
 * 
 * ATENÇÃO:
 * 
 * Nota1: este arquivo só pode conter definições de macros, pois ele foi incluido
 * no arquivo main.c que contém as definições PRAGMA e estas só podem ser utilizadas
 * antes da inclusão de arquivos headers. Veja Nota1 em arquivo main.c.
 */

#ifndef VERSAO_H
#define	VERSAO_H

#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
//Não deveria existir sob risco de infrigir a Nota1.

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//Esta diretiva será configurada pela IDE quando configuramos o modelo de PIC nas opções de projeto.
//No entanto, é interessante definí-la aqui também (de forma correta) para que o compilador reconheça as variáveis definidas nesse modelo de PIC.
//#define _PIC16F886_H_

//As diretivas a seguir definem a placa de hardware para o qual o software 
//será compilado. Apenas uma delas pode estar definida.
    
//#define _HARDWARE_2013_ //PIC16F876A (não esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).
#define _HARDWARE_2016_ //PIC16F886  (não esquecer de configurar o modelo do microcontrolador nas propriedades do projeto).

//Define uma string para a versão do software.
//Formato: AAAAMMDDvX.YY
//AAAA = Ano
//MM   = Mes
//DD   = Dia
//X.YY = Numeração (atualmente X=1 e Y=700)
#define _VER_STR_VERSAO_ "20250822v1.726"
  
#ifdef _HARDWARE_2013_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "A"
#endif 
#ifdef _HARDWARE_2016_
  #define _VER_STR_HARDWARE_NOVO_ANTIGO_ "N"
#endif

//Esta diretiva é definida pela IDE quando o projeto é executado em modo DEBUG.
//É interessante também habilitá-la aqui quando o projeto for executado em modo DEBUG
//pois assim a IDE mostra o código que será compilado.
//#define __DEBUG
    
//Esta diretiva somente será usada quando se desejar efetuar o DEBUG do LCD.
//Nessa caso, os comandos de escrita no LCD serão enviados para a porta serial 
//por nibble (4 bits) ao invés do byte inteiro.
//#define _DEBUG_LCD_CMD_

//===== Constantes Públicas ==================================================
//Não deveria existir sob risco de infrigir a Nota1.
    
//===== Tipos Públicos =======================================================
//Não deveria existir sob risco de infrigir a Nota1.
    
//===== Variaveis Públicas ===================================================
//Não deveria existir sob risco de infrigir a Nota1.
    
//===== Funcoes Públicas =====================================================
//Não deveria existir sob risco de infrigir a Nota1.
    
//============================================================================
    
#ifdef	__cplusplus
}
#endif

#endif	/* VERSAO_H */
