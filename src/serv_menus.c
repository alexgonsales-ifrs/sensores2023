/*****************************************************************************
 * File:   serv_menus.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/

//===== Includes =============================================================
#include <stdio.h> //NULL
#include "serv_menus.h"

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variáveis Públicas ===================================================

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variáveis Privadas ===================================================

#if defined (_HARDWARE_2013_)
  //Itens para o menu menu_cfg_quant_sensores:
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
        "1 sensor",   1,
        "2 sensores", 2,
        "3 sensores", 3,
        "4 sensores", 4  //não fechar virgula no último item
  };

  #elif defined (_HARDWARE_2016_)

  static TMenu menu_principal;
  static TMenu menu_cfg_tempo_aquisicao;
  static TMenu menu_cfg_quant_sensores;
  static TMenu menu_configuracoes;
  static TMenu menu_configuracoes_item2;
  
  void serv_menu_princ_inicializa(void);
  
 // TMenu menu_principal =           {0, 0, MENU_PRINCIPAL_TAM,           &menu_principal_itens, 0};
  //TMenu menu_cfg_tempo_aquisicao = {0, 0, MENU_CFG_TEMPO_AQUISICAO_TAM, 0, &menu_principal};
  
  //Os vetores de itens abaixo não foram inicializados com o terceiro campo (p_submenu)
  //pois o compilador gerava o erro 240 (too many initializers). Isso acontece
  //devido à forma como foram declarados os tipos TMenu e TMenuItem no arquivo util_menu.h.
  
  //Itens para o menu menu_principal: 
   static const TMenuItem menu_principal_itens[MENU_PRINCIPAL_TAM] = {
      {"1)Monitora",      0,0,0},
      {"2)Monit/Grava",   1,0,0},
      {"3)Ver Todos",     2,0,0},
      {"4)Max/Min",       3,0,0},
      {"5)Limpar",        4,0,0},
      {"6)Tempo Aquisi.", 5,&menu_cfg_tempo_aquisicao,0},
      {"7)Quant.Sens.",   6,&menu_cfg_quant_sensores,0},                 
      {"8)Enviar Dados",  7,0,0},   
      {"9)Configuracoes", 8,&menu_configuracoes,0}   //não fechar virgula no último item
  };

  //Itens para o menu menu_cfg_quant_sensores:
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
      {"1", 1,0,1},
      {"2", 2,0,1},
      {"3", 3,0,1},
      {"4", 4,0,1},
      {"5", 5,0,1},
      {"6", 6,0,1},
      {"7", 7,0,1},
      {"8", 8,0,1}  //não fechar virgula no último item
  };

  const TMenuItem menu_cfg_tempo_aquisicao_itens[MENU_CFG_TEMPO_AQUISICAO_TAM] = {
      //{"0.1 seg", TEMPO_01_SEGUNDO,0},
      //{"0.5 seg", TEMPO_05_SEGUNDO,0},
        {"1 seg",  ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO,   0,1},
        {"10 seg", ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS, 0,1},
        {"30 seg", ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS, 0,1},
        {"1 min",  ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO,    0,1},
        {"10 min", ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS,  0,1},
        {"30 min", ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS,  0,1},
        {"1 hora", ADCON_CFG_TEMPO_AQUISICAO_1_HORA,      0,1}  //não fechar virgula no último item
  };
  
  //Itens para o menu menu_configuracoes:
  const TMenuItem menu_configuracoes_itens[MENU_CONFIGURACOES_TAM] = {
      {"Item 1", 1, 0,1},
      {"Item 2", 2, &menu_configuracoes_item2,1},
      {"Item 3", 3, 0,0},
      {"Item 4", 4, 0,0}  //não fechar virgula no último item
  };
  
  //Itens para o menu menu_configuracoes_item1:
  const TMenuItem menu_configuracoes_item2_itens[MENU_CONFIGURACOES_ITEM1_TAM] = {
      {"Item2.1", 1, 0,1},
      {"Item2.2", 2, 0,1},
      {"Item2.3", 3, 0,1}  //não fechar virgula no último item
  };

  //Ocupou 73 words de memória de programa (2025-04-06).
  //const char* SERV_MENU_TITULO_MENU_PRINCIPAL = "Princ.";
  //const char* SERV_MENU_TITULO_MENU_QUANT_SENSORES = "Qt. Sens.";
  //const char* SERV_MENU_TITULO_MENU_TEMPO_AQUISICAO = "Tempo Aquis.";
    
  #endif
    
//===== Declaração das Funções Privadas ======================================

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

void serv_menu_princ_inicializa(void) {
    //Inicializa os menus:
  
    //Ocupou apenas 44 words de memória de programa.
    menu_principal.str_titulo           = "Princ.";
    menu_cfg_quant_sensores.str_titulo  = "Qt. Sens.";
    menu_cfg_tempo_aquisicao.str_titulo = "Tempo Aquis.";
    menu_configuracoes.str_titulo       = "Configs.";

    //Ocupa 15 words e mais os 73 words de programa da declaração const, totalizando 88 words.
    //menu_principal.str_titulo           = &SERV_MENU_TITULO_MENU_PRINCIPAL;
    //menu_cfg_quant_sensores.str_titulo  = &SERV_MENU_TITULO_MENU_QUANT_SENSORES;
    //menu_cfg_tempo_aquisicao.str_titulo = &SERV_MENU_TITULO_MENU_TEMPO_AQUISICAO;
    
    menu_init(&menu_principal,           menu_principal_itens,           MENU_PRINCIPAL_TAM, NULL);
    menu_init(&menu_cfg_quant_sensores,  menu_cfg_quant_sensores_itens,  MENU_CFG_QUANT_SENSORES_TAM,  &menu_principal);
    menu_init(&menu_cfg_tempo_aquisicao, menu_cfg_tempo_aquisicao_itens, MENU_CFG_TEMPO_AQUISICAO_TAM, &menu_principal);
    menu_init(&menu_configuracoes,       menu_configuracoes_itens,       MENU_CONFIGURACOES_TAM,       &menu_principal);
    //menu_init(&menu_configuracoes_item2, menu_configuracoes_item2_itens, MENU_CONFIGURACOES_ITEM1_TAM, &menu_configuracoes);
    
    //menu_add_submenu(&menu_principal, 5, &menu_cfg_tempo_aquisicao);
    
    //menu_principal.itens[5].p_submenu = &menu_cfg_tempo_aquisicao;
    //menu_cfg_tempo_aquisicao.p_supermenu = &menu_principal;
        
    menu_p_menu_ativo = &menu_principal;
    
}

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
