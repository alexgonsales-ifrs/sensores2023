/* 
 * File:   menu_principal.h
 * Author: ti
 *
 * Created on 30 de Dezembro de 2013, 23:59
 */

#ifndef MENU_PRINCIPAL_H
#define	MENU_PRINCIPAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "versao.h"
#include "menu.h"
#include "cfg_tempo_amostra.h"
    
#ifdef _ENVIA_DADOS_SERIAL_
  #define MENU_PRINCIPAL_TAM  8 // tamanho do menu inicial
#else
  #define MENU_PRINCIPAL_TAM  7 // tamanho do menu inicial
#endif

  TMenu menu_principal;
  
  const TMenuItem menu_principal_itens[MENU_PRINCIPAL_TAM] = {
        "1)Monitora",      0,
        "2)Monit/Grava",   1,
        "3)Ver Todos",     2,
        "4)Max/Min",       3,
        "5)Limpar",        4,
        "6)Tempo Amostra", 5,
        "7)Quant.Sensors", 6                 //não fechar virgula aqui
        #ifdef _ENVIA_DADOS_SERIAL_
         ,"8)Enviar Dados", 7
        #endif

  };

  #define MENU_CFG_QUANT_SENSORES_TAM  4 // tamanho do menu.
    
  TMenu menu_cfg_quant_sensores;
      
  //const TSensMenuQuantSensores sens_menu_quant_sensores[SENS_TAM_MENU_QUANT_SENSORES] = {
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
        "1 sensor",   1,
        "2 sensores", 2,
        "3 sensores", 3,
        "4 sensores", 4
  };

  #define MENU_CFG_TEMPO_AMOSTRA_TAM 7// tamanho do menu.

  TMenu menu_cfg_tempo_amostra;
 
  const TMenuItem menu_cfg_tempo_amostra_itens[MENU_CFG_TEMPO_AMOSTRA_TAM] = {
        //"0.1 seg", TEMPO_01_SEGUNDO,
        //"0.5 seg", TEMPO_05_SEGUNDO,
        "1 seg",  CFG_TEMPO_AMOSTRA_1_SEGUNDO,
        "10 seg", CFG_TEMPO_AMOSTRA_10_SEGUNDOS,
        "30 seg", CFG_TEMPO_AMOSTRA_30_SEGUNDOS,
        "1 min",  CFG_TEMPO_AMOSTRA_1_MINUTO,
        "10 min", CFG_TEMPO_AMOSTRA_10_MINUTOS,
        "30 min", CFG_TEMPO_AMOSTRA_30_MINUTOS,
        "1 hora", CFG_TEMPO_AMOSTRA_1_HORA
  };
  
#ifdef	__cplusplus
}
#endif

#endif	/* MENU_PRINCIPAL_H */