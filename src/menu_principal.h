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
#include "adcon.h"
    
//Quantidade de itens do Menu Principal.
#ifdef _ENVIA_DADOS_SERIAL
  #define MENU_PRINCIPAL_TAM  8 // tamanho do menu inicial
#else
  #define MENU_PRINCIPAL_TAM  7 // tamanho do menu inicial
#endif

  //Quantidade de itens do menu Quantidade de Sensores.
  #define MENU_CFG_QUANT_SENSORES_TAM  4 // tamanho do menu.

  //Quantidade de itens do menu Tempo Amostra.
  #define MENU_CFG_TEMPO_AMOSTRA_TAM 7// tamanho do menu.

  TMenu menu_principal;
  
  const TMenuItem menu_principal_itens[8] = {
      {"1)Monitora",      0},
      {"2)Monit/Grava",   1},
      {"3)Ver Todos",     2},
      {"4)Max/Min",       3},
      {"5)Limpar",        4},
      {"6)Tempo Amostra", 5},
      {"7)Quant.Sensors", 6},                 //n�o fechar virgula aqui
      {"8)Enviar Dados", 7}
  };
      
  TMenu menu_cfg_tempo_amostra;
  
  //const TSensMenuQuantSensores sens_menu_quant_sensores[SENS_TAM_MENU_QUANT_SENSORES] = {
  const TMenuItem menu_cfg_quant_sensores_itens[MENU_CFG_QUANT_SENSORES_TAM] = {
        "1 sensor",   1,
        "2 sensores", 2,
        "3 sensores", 3,
        "4 sensores", 4
  };

  TMenu menu_cfg_quant_sensores;
  
  const TMenuItem menu_cfg_tempo_amostra_itens[MENU_CFG_TEMPO_AMOSTRA_TAM] = {
        //"0.1 seg", TEMPO_01_SEGUNDO,
        //"0.5 seg", TEMPO_05_SEGUNDO,
        "1 seg",  ADCON_CFG_TEMPO_AMOSTRA_1_SEGUNDO,
        "10 seg", ADCON_CFG_TEMPO_AMOSTRA_10_SEGUNDOS,
        "30 seg", ADCON_CFG_TEMPO_AMOSTRA_30_SEGUNDOS,
        "1 min",  ADCON_CFG_TEMPO_AMOSTRA_1_MINUTO,
        "10 min", ADCON_CFG_TEMPO_AMOSTRA_10_MINUTOS,
        "30 min", ADCON_CFG_TEMPO_AMOSTRA_30_MINUTOS,
        "1 hora", ADCON_CFG_TEMPO_AMOSTRA_1_HORA
  };

extern void menu_princ_inicializa(void);
  
#ifdef	__cplusplus
}
#endif

#endif	/* MENU_PRINCIPAL_H */