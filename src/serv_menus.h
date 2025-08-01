/* 
 * File:   serv_menus.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 23:59
 * 
 * Este arquivo define a estrutura de menus da aplicação:
 * 
 * Principal
 *   {"1)Monitora",      0},
 *   {"2)Monit/Grava",   1},
 *   {"3)Ver Todos",     2},
 *   {"4)Max/Min",       3},
 *   {"5)Limpar",        4},
 *   {"6)Tempo Aquisi.",  5},
 *     "1 seg",  ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO,
 *     "10 seg", ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS,
 *     "30 seg", ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS,
 *     "1 min",  ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO,
 *     "10 min", ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS,
 *     "30 min", ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS,
 *     "1 hora", ADCON_CFG_TEMPO_AQUISICAO_1_HORA  //não fechar virgula no último item
 *   {"7)Quant.Sens.",   6},                 
 *         "1", 1,
 *         "2", 2,
 *         "3", 3,
 *         "4", 4,
 *         "5", 5,
 *         "6", 6,
 *         "7", 7,
 *         "8", 8  //não fechar virgula no último item
 *   {"8)Enviar Dados",  7}   //não fechar virgula no último item
 *   {"9)Configuracoes
 *       Item 1
 *         Item1 1
 *         Item1 2
 *         Item1 3
 *       Item 2
 *       Item 3
 *       Item 4
 * 
 *///
//
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
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Constantes Públicas ==================================================
    

  //Quantidade de itens do menu Quantidade de Sensores.
  #if defined (_HARDWARE_2013_)
    #define MENU_CFG_QUANT_SENSORES_TAM  4 // tamanho do menu.
  #elif defined (_HARDWARE_2016_)
    #define MENU_PRINCIPAL_TAM  9 // tamanho do menu principal
    #define MENU_CFG_TEMPO_AQUISICAO_TAM 7// tamanho do menu.
    #define MENU_CFG_QUANT_SENSORES_TAM  8 // tamanho do menu.
    #define MENU_CONFIGURACOES_TAM       4 //
    #define MENU_CONFIGURACOES_ITEM2_TAM 3 //
  #endif


typedef enum {
        SERV_MENU_ACAO_EXEC_NULL,
        SERV_MENU_ACAO_EXEC_MONITORA,
        SERV_MENU_ACAO_EXEC_MONITORA_GRAVA,
        SERV_MENU_ACAO_EXEC_VER_AQUISICOES,
        SERV_MENU_ACAO_EXEC_MOSTRA_MAX_MIN,
        SERV_MENU_ACAO_EXEC_LIMPAR,
        SERV_MENU_ACAO_EXEC_ENVIAR_DADOS
        //Não colocar vírgula no último item.
        } TServMenuCodigoAcaoExec;

typedef enum {
        SERV_MENU_ACAO_CFG_NULL,
        SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, 
        SERV_MENU_ACAO_CFG_QUANT_SENSORES
        //Não colocar vírgula no último item.
        } TServMenuCodigoAcaoCfg;
  
//===== Tipos Públicos =======================================================

  
//===== Variaveis Públicas ===================================================
//extern TMenu    *serv_menu_p_menu_ativo; //ponteiro para o menu ativo.
extern TMenuSystem serv_menu_raiz;

//===== Funcoes Públicas =====================================================
  
  extern void serv_menu_princ_inicializa(void);
  
#ifdef	__cplusplus
}
#endif

#endif	/* SERV_MENUS_H */