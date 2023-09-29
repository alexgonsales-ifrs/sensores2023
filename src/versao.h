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

#define _MODULO_NOVO_ //PIC16F886 (não esquecer de configurar o modelo nas propriedades do projeto).

//Esta diretiva será configurada pela IDE quando configuramos o modelo de PIC nas opções de projeto.
//No entanto, é interessante definí-la aqui também (de forma correta) para que o compilador reconheça as variáveis definidas nesse modelo de PIC.
//#define _PIC16F886_H_
    
//#define _MODULO_ANTIGO_ //PIC16F876A (não esquecer de configurar o modelo nas propriedades do projeto).
    
//Habilita a função "Enviar Dados" que envia pela serial os dados que estão gravados na memória RAM.
//Esta diretiva se chamava _DEBUG_ e foi modificada para _ENVIA_DADOS_SERIAL_.
//Antes de habilitar esta diretiva, verificar qual modelo de PIC está sendo usado para enviar dados pela serial.
//#define _ENVIA_DADOS_SERIAL_

//Esta diretiva é definida pelo compilador quando o projeto é executado em modo DEBUG.
//É interessante também habilitá-la aqui quando o projeto for executado em modo DEBUG,
//pois assim a IDE mostra o código que será compilado.
//#define __DEBUG
    
//Esta diretiva somente será usada quando o projeto estiver em modo __DEBUG.
//Nessa caso, se esta diretiva estiver ativada, então os comandos de escrita no LCD
//serão enviadas por nibble (4 bits) ao invés do byte inteiro.
//#define _DEBUG_LCD_CMD_

//#define _MQ_
#define _LM35_

#ifdef	__cplusplus
}
#endif

#endif	/* VERSAO_H */

