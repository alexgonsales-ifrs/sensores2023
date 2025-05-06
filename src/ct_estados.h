/* 
 * File:   ct_estados.h
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
//===== Definições Públicas ==================================================
//============================================================================
// Definições e declarações públicas do módulo.

//===== Constantes Públicas ==================================================
    
//===== Tipos Públicos =======================================================

  /* Todos os estados possíveis para a máquina de estados.*/
  typedef enum {
        EST_ESTADO_NULL,
        EST_ESTADO_INICIAL,
        EST_ESTADO_TRATA_MENUS,
        EST_ESTADO_MONITORA,
        EST_ESTADO_MONITORA_GRAVA,
        EST_ESTADO_VER_AQUISICOES,
        EST_ESTADO_MOSTRA_MAX_MIN,
        EST_ESTADO_LIMPAR,
        EST_ESTADO_ENVIAR_DADOS
  } TEstado;

//===== Variaveis Públicas ===================================================

  //Variável global que matem o estado atual da máquina de estados.  
  extern TEstado est_estado_atual;
  
//===== Funcoes Públicas =====================================================

  //Função que executa um step na máquina de estados, quando acontece um evento de botão.
  extern void est_evento_botao(TBotao botao);
  
  //Função que executa um step na máquina de estados, quando acontece o evento Timer0.
  extern void est_evento_timer0();

#ifdef	__cplusplus
}
#endif

#endif	/* CT_ESTADOS_H */
