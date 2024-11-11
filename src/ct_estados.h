/* 
 * File:   estados.h
 * Author: ifrs
 *
 * Created on 14 de Abril de 2015, 18:44
 */
#ifndef CT_ESTADOS_H
#define	CT_ESTADOS_H
#ifdef	__cplusplus
extern "C" {
#endif

//===== Includes =============================================================
    
  #include "base_botoes.h" //Utiliza TBotao.

//============================================================================
//===== Defini��es P�blicas ==================================================
//============================================================================
// Defini��es e declara��es p�blicas do m�dulo.

//===== Constantes P�blicas ==================================================
    
//===== Tipos P�blicos =======================================================

  /* Todos os estados poss�veis para a m�quina de estados.
   Os Estados que contem MENU no nome representam o menu principal ou algum sub-menu.*/
  typedef enum {
        EST_ESTADO_NULL,
        EST_ESTADO_INICIAL,
        EST_ESTADO_MENU_PRINCIPAL,
        EST_ESTADO_MONITORA,
        EST_ESTADO_MONITORA_GRAVA,
        EST_ESTADO_VER_AQUISICOES,
        EST_ESTADO_MOSTRA_MAX_MIN,
        EST_ESTADO_LIMPAR,
        EST_ESTADO_MENU_CONF_TEMPO_AQUISICAO, //Menu
        EST_ESTADO_MENU_CONF_QUANT_SENSORES,  //Menu
        EST_ESTADO_ENVIAR_DADOS,
                
        EST_ESTADO_MENU_CONFIGURACOES,        //Menu Configuracoes
        EST_ESTADO_MENU_CONFIGURACOES_ITEM2,
                
        EST_ESTADO_TRATA_MENU
  } TEstado;

//===== Variaveis P�blicas ===================================================
  
//===== Funcoes P�blicas =====================================================

  //Vari�vel global que matem o estado atual da m�quina de estados.  
  extern TEstado est_estado_atual;

  //Fun��o principal que controla a m�quina de estados.
  extern void est_maquina(TBotao botao);

#ifdef	__cplusplus
}
#endif

#endif	/* CT_ESTADOS_H */
