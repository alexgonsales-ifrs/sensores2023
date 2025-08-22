/* 
 * File:   serv_menus.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 23:59
 * 
 * Este m�dulo define a seguinte estrutura de menus da aplica��o:
 * 
 * Principal
 *   1)Monitora
 *   2)Monit/Grava
 *   3)Ver Todos
 *   4)Max/Min
 *   5)Limpar
 *   6)Enviar Dados
 *   7)Configura
 *     1)Monit.
 *       1)Tempo Aquisi.
 *         1 seg, 10 seg, 30 seg, 1 min, 10 min, 30 min, 1 hora
 *       2)Quant. Sens.
 *         1, 2, 3, 4, 5, 6, 7, 8
 *     2)Portas
 *     3)Serial
 *     4)Display
 * 
 */

#ifndef SERV_MENUS_H
#define	SERV_MENUS_H

  #ifdef	__cplusplus
  extern "C" {
  #endif

//===== Includes =============================================================
    
  #include "versao.h"
  #include "util_menu.h"
  #include "base_adcon.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================
    

  //Quantidade de itens do menu Quantidade de Sensores.
  #if defined (_HARDWARE_2013_)
    #define MENU_CFG_QT_SENS_TAM  4 // tamanho do menu.
  #elif defined (_HARDWARE_2016_)
    #define MENU_PRINCIPAL_TAM    7 // tamanho do menu principal
    #define MENU_CFG_TP_AQUIS_TAM 7 // tamanho do menu.
    #define MENU_CFG_QT_SENS_TAM  8 // tamanho do menu.
    #define MENU_CFG_TAM          4 //
    #define MENU_CFG_MONIT_TAM    2 

    #define MENU_CONFIGURACOES_ITEM2_TAM 3 //
  #endif

//C�digos de a��o para cada item de menu de a��o do sistema de menus.
typedef enum {
        SERV_MENU_ACAO_EXEC_NULL,
        SERV_MENU_ACAO_EXEC_MONITORA,
        SERV_MENU_ACAO_EXEC_MONITORA_GRAVA,
        SERV_MENU_ACAO_EXEC_VER_AQUISICOES,
        SERV_MENU_ACAO_EXEC_MOSTRA_MAX_MIN,
        SERV_MENU_ACAO_EXEC_LIMPAR,
        SERV_MENU_ACAO_EXEC_ENVIAR_DADOS
        //N�o colocar v�rgula no �ltimo item.
        } TServMenuCodigoAcaoExec;

//C�digos de a��o para cada item de menu de configura��o do sistema de menus.
typedef enum {
        SERV_MENU_ACAO_CFG_NULL,
        SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, 
        SERV_MENU_ACAO_CFG_QUANT_SENSORES
        //N�o colocar v�rgula no �ltimo item.
        } TServMenuCodigoAcaoCfg;
  
//===== Tipos P�blicos =======================================================

  
//===== Variaveis P�blicas ===================================================

//Vari�vel que mant�m o ponto de entrada para a estrutura de menus.
extern TMenuSystem serv_menu_raiz;

//===== Funcoes P�blicas =====================================================
  
extern void serv_menu_princ_inicializa(void);
  
#ifdef	__cplusplus
}
#endif

#endif	/* SERV_MENUS_H */