/* 
 * File:   serv_menus.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 23:59
 * 
 * Este arquivo define a estrutura de menus da aplica��o:
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
 *     "1 hora", ADCON_CFG_TEMPO_AQUISICAO_1_HORA  //n�o fechar virgula no �ltimo item
 *   {"7)Quant.Sens.",   6},                 
 *         "1", 1,
 *         "2", 2,
 *         "3", 3,
 *         "4", 4,
 *         "5", 5,
 *         "6", 6,
 *         "7", 7,
 *         "8", 8  //n�o fechar virgula no �ltimo item
 *   {"8)Enviar Dados",  7}   //n�o fechar virgula no �ltimo item
 *   {"9)Configuracoes
 *       Item 1
 *         Item1 1
 *         Item1 2
 *         Item1 3
 *       Item 2
 *       Item 3
 *       Item 4
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
    
  //Quantidade de itens do Menu Principal.
  #define MENU_PRINCIPAL_TAM  9 // tamanho do menu principal

  //Quantidade de itens do menu Quantidade de Sensores.
  #if defined (_HARDWARE_2013_)
    #define MENU_CFG_QUANT_SENSORES_TAM  4 // tamanho do menu.
  #elif defined (_HARDWARE_2016_)
    #define MENU_CFG_QUANT_SENSORES_TAM  8 // tamanho do menu.
    #define MENU_CONFIGURACOES_TAM       4 //
    #define MENU_CONFIGURACOES_ITEM1_TAM 3 //
  #endif

  //Quantidade de itens do menu Tempo Amostra.
  #define MENU_CFG_TEMPO_AQUISICAO_TAM 7// tamanho do menu.

  //Itens para o menu menu_principal: 
  const TMenuItem menu_principal_itens[MENU_PRINCIPAL_TAM] = {
      {"1)Monitora",      0},
      {"2)Monit/Grava",   1},
      {"3)Ver Todos",     2},
      {"4)Max/Min",       3},
      {"5)Limpar",        4},
      {"6)Tempo Aquisi.", 5},
      {"7)Quant.Sens.",   6},                 
      {"8)Enviar Dados",  7},   
      {"9)Configuracoes", 8}   //n�o fechar virgula no �ltimo item
  };

  #if defined (_HARDWARE_2013_)
  //Itens para o menu menu_cfg_quant_sensores:
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
        "1 sensor",   1,
        "2 sensores", 2,
        "3 sensores", 3,
        "4 sensores", 4  //n�o fechar virgula no �ltimo item
  };

  #elif defined (_HARDWARE_2016_)
  //Itens para o menu menu_cfg_quant_sensores:
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
      {"1", 1, 0},
      {"2", 2, 0},
      {"3", 3, 0},
      {"4", 4, 0},
      {"5", 5, 0},
      {"6", 6, 0},
      {"7", 7, 0},
      {"8", 8, 0}  //n�o fechar virgula no �ltimo item
  };
  
  //Itens para o menu menu_configuracoes:
  const TMenuItem menu_configuracoes_itens[MENU_CONFIGURACOES_TAM] = {
      {"Item 1", 1, 0},
      {"Item 2", 2, 0},
      {"Item 3", 3, 0},
      {"Item 4", 4, 0}  //n�o fechar virgula no �ltimo item
  };

  //Itens para o menu menu_configuracoes_item1:
  const TMenuItem menu_configuracoes_item1_itens[MENU_CONFIGURACOES_ITEM1_TAM] = {
      {"Item1 1", 1, 0},
      {"Item1 2", 2, 0},
      {"Item1 3", 3, 0}  //n�o fechar virgula no �ltimo item
  };

  #endif

  //Itens para o menu menu_cfg_tempo_aquisicao:
  const TMenuItem menu_cfg_tempo_aquisicao_itens[MENU_CFG_TEMPO_AQUISICAO_TAM] = {
      //{"0.1 seg", TEMPO_01_SEGUNDO,0},
      //{"0.5 seg", TEMPO_05_SEGUNDO,0},
        {"1 seg",  ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO,   0},
        {"10 seg", ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS, 0},
        {"30 seg", ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS, 0},
        {"1 min",  ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO,    0},
        {"10 min", ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS,  0},
        {"30 min", ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS,  0},
        {"1 hora", ADCON_CFG_TEMPO_AQUISICAO_1_HORA,      0}  //n�o fechar virgula no �ltimo item
  };

//===== Tipos P�blicos =======================================================

  
//===== Variaveis P�blicas ===================================================
  TMenu menu_principal;
  TMenu menu_cfg_quant_sensores;
  TMenu menu_cfg_tempo_aquisicao;
  
  TMenu menu_configuracoes;
  TMenu menu_configuracoes_item1;
    
//===== Funcoes P�blicas =====================================================
  
  extern void serv_menu_princ_inicializa(void);
  
#ifdef	__cplusplus
}
#endif

#endif	/* SERV_MENUS_H */