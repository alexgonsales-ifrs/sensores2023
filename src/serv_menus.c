/*****************************************************************************
 * File:   serv_menus.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 ****************************************************************************/


//===== Includes =============================================================
#include "serv_menus.h"

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================

//===== Constantes P�blicas ==================================================

//===== Vari�veis P�blicas ===================================================

//============================================================================
//===== Defini��es e Declara�oes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Vari�veis Privadas ===================================================

//===== Declara��o das Fun��es Privadas ======================================

//============================================================================
//===== Defini��o (implementa��o) das Fun��es P�blicas =======================
//============================================================================

void serv_menu_princ_inicializa(void) {
    //Inicializa os menus:
    menu_init(&menu_principal, menu_principal_itens, MENU_PRINCIPAL_TAM);
    menu_init(&menu_cfg_quant_sensores, menu_cfg_quant_sensores_itens, MENU_CFG_QUANT_SENSORES_TAM);
    menu_init(&menu_cfg_tempo_aquisicao, menu_cfg_tempo_aquisicao_itens, MENU_CFG_TEMPO_AQUISICAO_TAM);
    
    menu_init(&menu_configuracoes, menu_configuracoes_itens, MENU_CONFIGURACOES_TAM);
    menu_init(&menu_configuracoes_item1, menu_configuracoes_item1_itens, MENU_CONFIGURACOES_ITEM1_TAM);
}

//============================================================================
//===== Defini��o (implementa��o) das Fun��es Privadas =======================
//============================================================================

//===== Final do Arquivo =====================================================
