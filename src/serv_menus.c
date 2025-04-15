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

TMenuRaiz serv_menu_raiz;

//TMenu *serv_menu_p_menu_ativo; //ponteiro para o menu ativo.


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

  static TMenu menu_configuracoes       = {0,0,0,0,"Config."};
  static TMenu menu_principal           = {0,0,0,0,"Principal"};
  static TMenu menu_cfg_tempo_aquisicao = {0,0,0,0,"Tempo Aquis."};
  static TMenu menu_cfg_quant_sensores  = {0,0,0,0,"Quant. Sens."};
  
  //static TMenu menu_configuracoes_item2;
  
  // TMenu menu_principal =           {0, 0, MENU_PRINCIPAL_TAM,           &menu_principal_itens, 0};
  //TMenu menu_cfg_tempo_aquisicao = {0, 0, MENU_CFG_TEMPO_AQUISICAO_TAM, 0, &menu_principal};
  
  //Os vetores de itens abaixo não foram inicializados com o terceiro campo (p_submenu)
  //pois o compilador gerava o erro 240 (too many initializers). Isso acontece
  //devido à forma como foram declarados os tipos TMenu e TMenuItem no arquivo util_menu.h.
  
//Itens para o menu menu_configuracoes:
  const TMenuItem menu_configuracoes_itens[MENU_CONFIGURACOES_TAM] = {
      {"Item 1", MENU_TIPO_ITEM_CFG,     SERV_MENU_ACAO_CFG_NULL, 1},
      //{"Item 2", MENU_TIPO_ITEM_SUBMENU, 0,                  &menu_configuracoes_item2},
      {"Item 3", MENU_TIPO_ITEM_CFG,     SERV_MENU_ACAO_CFG_NULL, 3},
      {"Item 3", MENU_TIPO_ITEM_CFG,     SERV_MENU_ACAO_CFG_NULL, 3},
      {"Item 4", MENU_TIPO_ITEM_CFG,     SERV_MENU_ACAO_CFG_NULL, 4}  //não fechar virgula no último item
  };
  
  //Itens para o menu menu_principal: 
   static const TMenuItem menu_principal_itens[MENU_PRINCIPAL_TAM] = {
      {"1)Monitora",      MENU_TIPO_ITEM_ACAO,    SERV_MENU_ACAO_EXEC_MONITORA,       0 },
      {"9)Configura", MENU_TIPO_ITEM_SUBMENU, 0,                                  (uint16_t)&menu_configuracoes},
      {"7)Quant.Sens.",   MENU_TIPO_ITEM_SUBMENU, 0,                                 (uint16_t)&menu_cfg_quant_sensores},
      {"6)Tempo Aquisi.", MENU_TIPO_ITEM_SUBMENU, 0,                                 (uint16_t)&menu_cfg_tempo_aquisicao},

      {"2)Monit/Grava",   MENU_TIPO_ITEM_ACAO,    SERV_MENU_ACAO_EXEC_MONITORA_GRAVA, 0 },
      {"3)Ver Todos",     MENU_TIPO_ITEM_ACAO,    SERV_MENU_ACAO_EXEC_VER_AQUISICOES, 0 },
      {"4)Max/Min",       MENU_TIPO_ITEM_ACAO,    SERV_MENU_ACAO_EXEC_MOSTRA_MAX_MIN, 0 },
      {"5)Limpar",        MENU_TIPO_ITEM_ACAO,    SERV_MENU_ACAO_EXEC_LIMPAR,         0 },

      {"8)Enviar Dados",  MENU_TIPO_ITEM_ACAO,    SERV_MENU_ACAO_EXEC_ENVIAR_DADOS,   0}
      
      
      //não fechar virgula no último item
  };

  //Itens para o menu menu_cfg_quant_sensores:
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
      {"1", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 1},
      {"2", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 2},
      {"3", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 3},
      {"4", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 4},
      {"5", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 5},
      {"6", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 6},
      {"7", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 7},
      {"8", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_QUANT_SENSORES, 8}  //não fechar virgula no último item
  };

  const TMenuItem menu_cfg_tempo_aquisicao_itens[MENU_CFG_TEMPO_AQUISICAO_TAM] = {
      //{"0.1 seg", TEMPO_01_SEGUNDO,0},
      //{"0.5 seg", TEMPO_05_SEGUNDO,0},
        {"1 seg",  MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO},
        {"10 seg", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS},
        {"30 seg", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS},
        {"1 min",  MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO},
        {"10 min", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS},
        {"30 min", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS},
        {"1 hora", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_1_HORA}  //não fechar virgula no último item
  };
  
  
  
  /*
  //Itens para o menu menu_configuracoes_item1:
  const TMenuItem menu_configuracoes_item2_itens[MENU_CONFIGURACOES_ITEM2_TAM] = {
      {"Item2.1", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_ITEM2, 1},
      {"Item2.2", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_ITEM2, 2},
      {"Item2.3", MENU_TIPO_ITEM_CFG, SERV_MENU_ACAO_CFG_ITEM2, 3}  //não fechar virgula no último item
  };
*/
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
    //menu_configuracoes.str_titulo       = "Configs.";
    //menu_cfg_quant_sensores.str_titulo  = "Qt. Sens.";
    //menu_cfg_tempo_aquisicao.str_titulo = "Tempo Aquis.";
    //menu_principal.str_titulo           = "Princ.";
    
    //menu_configuracoes_item2.str_titulo       = "Item 2.";

    //Ocupa 15 words e mais os 73 words de programa da declaração const, totalizando 88 words.
    //menu_principal.str_titulo           = &SERV_MENU_TITULO_MENU_PRINCIPAL;
    //menu_cfg_quant_sensores.str_titulo  = &SERV_MENU_TITULO_MENU_QUANT_SENSORES;
    //menu_cfg_tempo_aquisicao.str_titulo = &SERV_MENU_TITULO_MENU_TEMPO_AQUISICAO;
    
    menu_init(&menu_configuracoes,       &(menu_configuracoes_itens[0]),       MENU_CONFIGURACOES_TAM,       &menu_principal);
    menu_init(&menu_principal,           &(menu_principal_itens[0]),           MENU_PRINCIPAL_TAM,           NULL);
    menu_init(&menu_cfg_quant_sensores,  &(menu_cfg_quant_sensores_itens[0]),  MENU_CFG_QUANT_SENSORES_TAM,  &menu_principal);
    menu_init(&menu_cfg_tempo_aquisicao, &(menu_cfg_tempo_aquisicao_itens[0]), MENU_CFG_TEMPO_AQUISICAO_TAM, &menu_principal);
    
    //menu_init(&menu_configuracoes_item2, menu_configuracoes_item2_itens, MENU_CONFIGURACOES_ITEM2_TAM, &menu_configuracoes);
    
    //menu_add_submenu(&menu_principal, 5, &menu_cfg_tempo_aquisicao);
    
    //menu_principal.itens[5].p_submenu = &menu_cfg_tempo_aquisicao;
    //menu_cfg_tempo_aquisicao.p_supermenu = &menu_principal;
    //menu_raiz_init(&serv_menu_raiz, &menu_principal );
    
    menu_raiz_init(&serv_menu_raiz, &menu_principal);
    
    //serv_menu_p_menu_ativo = &menu_principal;
    
}

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
