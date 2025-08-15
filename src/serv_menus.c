/*****************************************************************************
 * File:   serv_menus.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * 2025-08-01_a_08 (mariana): modificados menus e submenus. Reenviado por alexdg.
 * 
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

//Variável que mantém o ponto de entrada para a estrutura de menus.
TMenuSystem serv_menu_raiz;

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variáveis Privadas ===================================================

#if defined (_HARDWARE_2013_)
  //Itens para o menu menu_cfg_quant_sensores:
  const TMenuItem menu_cfg_qt_sens_itens[MENU_CFG_QT_SENS_TAM] = {
        "1 sensor",   1,
        "2 sensores", 2,
        "3 sensores", 3,
        "4 sensores", 4  //não fechar virgula no último item
  };

  #elif defined (_HARDWARE_2016_)

  //Vetores com os itens de cada menu. 
  //Apenas declara vetores que serão definidos (e inicializados) mais adiante neste arquivo.
  static const TMenuItem menu_principal_itens[];
  static const TMenuItem menu_cfg_itens[];
  static const TMenuItem menu_cfg_monit_itens[];
  static const TMenuItem menu_cfg_tp_aquis_itens[];
  static const TMenuItem menu_cfg_qt_sens_itens[];
  
  //Menus/submenus:                 tipo,index_active,quant_itens,             pv_itens,                    p_supermenu,     str_titulo
  static TMenu menu_principal    = {MNU_TIPO_GERAL, 0,MENU_PRINCIPAL_TAM,    &(menu_principal_itens[0]),    NULL,            "Principal"   };
  static TMenu menu_cfg_tp_aquis = {MNU_TIPO_LISTA, 0,MENU_CFG_TP_AQUIS_TAM, &(menu_cfg_tp_aquis_itens[0]), &menu_principal, "Tempo Aquis."};
  static TMenu menu_cfg_qt_sens  = {MNU_TIPO_LISTA, 0,MENU_CFG_QT_SENS_TAM,  &(menu_cfg_qt_sens_itens[0]),  &menu_principal, "Quant. Sens."};
  static TMenu menu_cfg          = {MNU_TIPO_LISTA, 0,MENU_CFG_TAM,          &(menu_cfg_itens[0]),          &menu_principal, "Config."     };
  static TMenu menu_cfg_monit    = {MNU_TIPO_LISTA, 0,MENU_CFG_MONIT_TAM,    &(menu_cfg_monit_itens[0]),    &menu_cfg,       "Monit."      }; 
  
  //Itens para o menu menu_principal
   static const TMenuItem menu_principal_itens[MENU_PRINCIPAL_TAM] = {
    {"1)Monitora",     SERV_MENU_ACAO_EXEC_MONITORA,       0, 0 },
    {"2)Monit/Grava",  SERV_MENU_ACAO_EXEC_MONITORA_GRAVA, 0, 0 },
    {"3)Ver Todos",    SERV_MENU_ACAO_EXEC_VER_AQUISICOES, 0, 0 },
    {"4)Max/Min",      SERV_MENU_ACAO_EXEC_MOSTRA_MAX_MIN, 0, 0 },
    {"5)Limpar",       SERV_MENU_ACAO_EXEC_LIMPAR,         0, 0 },
    {"6)Enviar Dados", SERV_MENU_ACAO_EXEC_ENVIAR_DADOS,   0, 0 },
    {"7)Configura",    SERV_MENU_ACAO_CFG_NULL,            0, &menu_cfg}  //não fechar virgula no último item
  };
   
  //Itens para o menu Configuracoes.
  static const TMenuItem menu_cfg_itens[MENU_CFG_TAM] = {
    {"1)Monit.",  SERV_MENU_ACAO_CFG_NULL, 0, &menu_cfg_monit},
    {"2)Portas",  SERV_MENU_ACAO_CFG_NULL, 0, 0},
    {"3)Serial",  SERV_MENU_ACAO_CFG_NULL, 0, 0},
    {"4)Display", SERV_MENU_ACAO_CFG_NULL, 0, 0}  //não fechar virgula no último item
  };

  // Itens para o menu Configurações Monitoramento.
  static const TMenuItem menu_cfg_monit_itens[MENU_CFG_MONIT_TAM] = {
    {"1)Tempo Aquisi.", SERV_MENU_ACAO_CFG_NULL, 0, &menu_cfg_tp_aquis},
    {"2)Quant. Sens.",  SERV_MENU_ACAO_CFG_NULL, 0, &menu_cfg_qt_sens}
  };

  //Itens para o menu Configura Tempo de Aquisição.
  static const TMenuItem menu_cfg_tp_aquis_itens[MENU_CFG_TP_AQUIS_TAM] = {
    //{"0.1 seg", TEMPO_01_SEGUNDO,0},
    //{"0.5 seg", TEMPO_05_SEGUNDO,0},
    {"1 seg",   SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_1_SEGUNDO,  0},
    {"10 seg",  SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_10_SEGUNDOS,0},
    {"30 seg",  SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_30_SEGUNDOS,0},
    {"1 min",   SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_1_MINUTO,   0},
    {"10 min",  SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_10_MINUTOS, 0},
    {"30 min",  SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_30_MINUTOS, 0},
    {"1 hora",  SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO, ADCON_CFG_TEMPO_AQUISICAO_1_HORA,     0}  //não fechar virgula no último item
  };

  //Itens para o menu Configura Quantidade de Sensores.
  static const TMenuItem menu_cfg_qt_sens_itens[MENU_CFG_QT_SENS_TAM] = {
    {"1", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 1,0},
    {"2", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 2,0},
    {"3", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 3,0},
    {"4", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 4,0},
    {"5", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 5,0},
    {"6", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 6,0},
    {"7", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 7,0},
    {"8", SERV_MENU_ACAO_CFG_QUANT_SENSORES, 8,0} //não fechar virgula no último item
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
    //menu_configuracoes.str_titulo       = "Configs.";
    //menu_cfg_quant_sensores.str_titulo  = "Qt. Sens.";
    //menu_cfg_tempo_aquisicao.str_titulo = "Tempo Aquis.";
    //menu_principal.str_titulo           = "Princ.";
    
    //menu_configuracoes_item2.str_titulo       = "Item 2.";

    //Ocupa 15 words e mais os 73 words de programa da declaração const, totalizando 88 words.
    //menu_principal.str_titulo           = &SERV_MENU_TITULO_MENU_PRINCIPAL;
    //menu_cfg_quant_sensores.str_titulo  = &SERV_MENU_TITULO_MENU_QUANT_SENSORES;
    //menu_cfg_tempo_aquisicao.str_titulo = &SERV_MENU_TITULO_MENU_TEMPO_AQUISICAO;
    
    mnu_system_init(&serv_menu_raiz, &menu_principal);
    
}

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
