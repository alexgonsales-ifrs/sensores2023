/* 
 * File:   estados.h
 * Author: ifrs
 *
 * Created on 14 de Abril de 2015, 18:44
 */
#ifndef ESTADOS_H
#define	ESTADOS_H
#ifdef	__cplusplus
extern "C" {
#endif
    
  #include "botoes.h"

  /* Todos os estados poss�veis para a m�quina de estados.
   Os Estados que contem MENU no nome representam o menu principal ou algum sub-menu.*/
  typedef enum {
        EST_ESTADO_INICIAL,
        EST_ESTADO_MENU_PRINCIPAL,
        EST_ESTADO_MONITORA,
        EST_ESTADO_MONITORA_GRAVA,
        EST_ESTADO_MOSTRA_TODOS,
        EST_ESTADO_MOSTRA_MAX_MIN,
        EST_ESTADO_LIMPAR,
        EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA,
        EST_ESTADO_MENU_CONF_QUANT_SENSORES,
        EST_ESTADO_ENVIAR_DADOS,
        EST_ESTADO_PROTOCOLO_RS232   //Equipamento est� recebendo comando pela RS232 ou enviando dados.
  } TEstado;
  
  //Vari�vel global que matem o estado atual da m�quina de estados.  
  extern TEstado est_estado_atual;

  //Fun��o principal que controla a m�quina de estados.
  extern void est_maquina(TBotao botao);

#ifdef	__cplusplus
}
#endif

#endif	/* ESTADOS_H */
