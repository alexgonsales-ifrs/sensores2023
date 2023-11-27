/*
 * Este m�dulo implementa a m�quina de estados para o menu da interface do usu�rio.
 * 
 * Ao ser pressionado um bot�o, a seguinte sequ�ncia de chamadas de fun��es � executada:
 * 1) Chama a fun��o est_maquina(botao) que verifica o estado atual e 
 * 2) chama a fun��o espec�fica do estado, que executa a a��o de sa�da de transi��o do estado e
 * 3) chama a fun��o est_executa(novo_estado), que executa a a��o de entrada do novo estado e troca o estado.
 *  
 * est_maquina(TBotao) - � a funcao de entrada da m�quina de estados.
 * Toda transi��o de estado come�a por esta fun��o, que � chamada pela interrup��o que foi gerada pelo bot�o.
 * O bot�o que foi pressionado � passado como par�metro para a fun��o.
 * Esta fun��o somente verifica o estado atual da m�quina de estados e chama a fun��o espec�fica para tratar o estado,
 * passando adiante o bot�o que foi pressionado.
 * 
 * Fun��es espec�ficas de cada estado:
 * Cada estado possui uma fun��o espec�fica que executa as a��es de transi��o que ocorrem no estado.
 * As a��es executadas durante uma transi��o est�o implementadas nessas fun��es espec�ficas (origem da transi��o).
 * Por exemplo: a a��o "decrementar item menu" ao clicar no bot�o UP enquanto no estado EST_CONF_QUANT_SENSORES
 * est� implementada na fun��o est_estado_menu_conf_quant_sensores().
 * 
 * As a��es espec�ficas de cada estado* s�o listadas a seguir:
 *  
 * est_estado_inicial(TBotao botao);
 * est_estado_menu_principal(TBotao botao);
 * est_estado_monitora(TBotao botao);
 * est_estado_monitora_grava(TBotao botao);
 * est_estado_mostra_todos(TBotao botao);
 * est_estado_mostra_max_min(TBotao botao);
 * est_estado_limpar(TBotao opt);
 * est_estado_menu_conf_tempo_amostra(TBotao botao);
 * est_estado_menu_conf_quant_sensores(TBotao botao);
 * est_estado_enviar_dados(TBotao botao);
 *  
 * est_executa_acao_do(TEstado novo_estado) -
 * Esta fun��o executa as a��es "do" (fazer) de cada estado e 
 * realiza tamb�m a troca para o novo_estado.
 *  
 */

#include <xc.h>
#include <stdio.h>

#include "estados.h"
#include "versao.h"
#include "botoes.h" //ja incluido no .h.
#include "lcd.h"
#include "serv_adcon.h"
#include "serv_eeprom.h"
#include "serv_rs232.h"
#include "menu_principal.h"

/*********************************************************************
******************** Defini��es P�blicas do M�dulo *******************
*********************************************************************/

//Vari�vel global que matem o estado atual da m�quina de estados.  
TEstado est_estado_atual = EST_ESTADO_INICIAL;

/*********************************************************************
**************** Defini��es Privadas do M�dulo ***********************
*********************************************************************/

/******************** Variaveis Privadas ****************************/

//Mantem o indice da amostra que est� sendo mostrada no display quando
//estiver no estado EST_ESTADO_MOSTRA_TODOS.
static uint8_t est_mostra_todos_index_amostra = 0;

/******************* Fun��es Privadas ******************************/

//Fun��es espec�ficas de cada estado.
static void est_estado_inicial(TBotao botao);
static void est_estado_menu_principal(TBotao botao);
static void est_estado_monitora(TBotao botao);
static void est_estado_monitora_grava(TBotao botao);
static void est_estado_mostra_todos(TBotao botao);
static void est_estado_mostra_max_min(TBotao botao);
static void est_estado_limpar(TBotao opt);
static void est_estado_menu_conf_tempo_amostra(TBotao botao);
static void est_estado_menu_conf_quant_sensores(TBotao botao);
static void est_estado_enviar_dados(TBotao botao);

//Fun��o que executa a a��o "do" dos estados.
//o par�metro novo_estado � o estado que ser� executada a a��o "do".
static void est_executa_acao_do(TEstado novo_estado);

/*********************************************************************
********** Defini��es (implementa��o) das Fun��es P�blicas **********
*********************************************************************/ 

/**
 * Esta � a funcao de entrada da m�quina de estado. Toda transi��o de estado
 * come�a por esta fun��o.
 * Verifica o estado atual da m�quina e chama a fun��o espec�fica do estado.
 *  * @param botao eh o botao do equipamento que foi pressionado.
 */
void est_maquina(TBotao botao) {

    switch (est_estado_atual) {
        case EST_ESTADO_INICIAL:
             est_estado_inicial(botao);
        break;//EST_INICIO

        case EST_ESTADO_MENU_PRINCIPAL:
            est_estado_menu_principal(botao);
        break;//EST_MENU_PRINCIPAL:

        case EST_ESTADO_MONITORA:
            est_estado_monitora(botao);
        break; //EST_CAPTURA_E_MOSTRA

        case EST_ESTADO_MONITORA_GRAVA:
            //usando o timer 0, a funcao somente testa o stop e para o timer:

            //    if (opt == BTN_STOP) {
            //        INTCONbits.T0IE = 0;
            //        muda_estado(EST_MENU_PRINCIPAL);
            //    }
            est_estado_monitora(botao);
        break; //EST_CAPTURA_E_GRAVA

        case EST_ESTADO_MOSTRA_TODOS:
            est_estado_mostra_todos(botao);
        break; //EST_MOSTRA_TODOS

        case EST_ESTADO_MOSTRA_MAX_MIN:
            est_estado_mostra_max_min(botao);
        break; //EST_MOSTRA_MAX_MIN

        case EST_ESTADO_LIMPAR:
            est_estado_limpar(botao);
        break; //EST_LIMPAR

        case EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA:
            est_estado_menu_conf_tempo_amostra(botao);
        break; //EST_MENU_CONF_TEMPO_AMOSTRA

        case EST_ESTADO_MENU_CONF_QUANT_SENSORES:
            est_estado_menu_conf_quant_sensores(botao);
        break; //EST_MENU_CONF_QUANT_SENSORES
        
        #ifdef _ENVIA_DADOS_SERIAL_
        case EST_ESTADO_ENVIAR_DADOS:
            est_estado_enviar_dados(botao);
        break; //EST_ENVIAR_DADOS
        #endif

        default:
            break;

    }//switch(gl_estado)
    
}//est_maquina()

/*********************************************************************
********* Defini��es (implementa��o) das Fun��es Privadas  ***********
*********************************************************************/

/**
 * A partir do estado inicial muda para o estado EST_MENU_PRINCIPAL
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_inicial(TBotao botao) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
}//est_estado_inicial()

/**
 * Funcao que trata as transi��es do estado EST_MENU_PRINCIPAL.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_menu_principal(TBotao botao) {
  uint8_t index;

  switch (botao) {
    case BTN_UP:
      if (menu_dec_index(&menu_principal) >= 0) {
         est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
      }
    break;

    case BTN_DOWN:
      if ( menu_inc_index(&menu_principal) >= 0 ) {
        est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
      }
    break;

    case BTN_START:
      index = menu_get_index_nav(&menu_principal);
      menu_confirma_index(&menu_principal);
      switch (index) {
        case 0: //"Monitora"
          serv_adcon_amostra_print();
          est_executa_acao_do(EST_ESTADO_MONITORA);
          break;

        case 1: //"Monitora/Grava"
          serv_adcon_amostra_print_grava();
          est_executa_acao_do(EST_ESTADO_MONITORA_GRAVA);
          break;

        case 2: //"Ver Todos"
          est_executa_acao_do(EST_ESTADO_MOSTRA_TODOS);
          break;

        case 3: //"Max/Min"
          est_executa_acao_do(EST_ESTADO_MOSTRA_MAX_MIN);
          break;

        case 4: //"Limpar"
          est_executa_acao_do(EST_ESTADO_LIMPAR);
          break;

        case 5: //"Tempo Amostra"
          //2023-11-27
          //Atualiza o �ndice do menu conforme o valor de adcon_cfg_quant_sensores_atual.
          menu_set_value_indexes(&menu_cfg_tempo_amostra, adcon_cfg_tempo_amostra_atual);
          est_executa_acao_do(EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA);
          break;

        case 6: //"Quant Sensores"
          //2023-11-27
          //Atualiza o �ndice do menu conforme o valor de adcon_cfg_quant_sensores_atual.
          menu_set_value_indexes(&menu_cfg_quant_sensores, adcon_cfg_quant_sensores_atual);
          est_executa_acao_do(EST_ESTADO_MENU_CONF_QUANT_SENSORES);
          break;
          
          #ifdef _ENVIA_DADOS_SERIAL_
          case 7: //"Enviar Dados"
            est_executa_acao_do(EST_ESTADO_ENVIAR_DADOS);
            break;
          #endif
      }//switch (index)
    break; //BTN_START
    
    default:
      break;

  } //switch (botao)

}//est_estado_menu_principal()

/**
 * Funcao que trata as transi��es do estado EST_MENU_LIMPAR.
  * @param botao o bot�o que foi pressionado.
 */
static void est_estado_limpar(TBotao botao) {
  if (botao == BTN_STOP) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_menu_limpar()

/* Funcao que trata as transi��es do estado EST_MONITORA.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_monitora(TBotao botao) {
  if (botao == BTN_STOP) {
    INTCONbits.T0IE = 0;
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_monitora()

/**
 * Funcao que trata as transi��es do estado EST_MONITORA_GRAVA.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_monitora_grava(TBotao botao) {
  if (botao == BTN_STOP) {
    INTCONbits.T0IE = 0;
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_monitora()

/**
 * Funcao que trata as transi��es do estado EST_ESTADO_MOSTRA_TODOS.
 * Controla a navegacao por todos os valores ja gravados na EEPROM.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_mostra_todos(TBotao botao) {
  switch (botao) {
    case BTN_UP:
      if (est_mostra_todos_index_amostra > 0) {
        est_mostra_todos_index_amostra--;
        est_executa_acao_do(EST_ESTADO_MOSTRA_TODOS);
      }
      break;
    case BTN_DOWN:
      if (serv_adcon_testa_indice_amostra_valido(est_mostra_todos_index_amostra)) {
        est_mostra_todos_index_amostra++;
        est_executa_acao_do(EST_ESTADO_MOSTRA_TODOS);
      }
      break;
    case BTN_STOP:
      est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
      break;
    default:
      break;
  }//switch
}//est_estado_mostra_todos()

/**
 * Funcao que trata as transi��es do estado EST_MOSTRA_MAX_MIN.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_mostra_max_min(TBotao botao) {
  if (botao == BTN_STOP) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_mostra_max_min()

/**
 * Funcao que trata as transi��es do estado EST_MENU_CONF_TEMPO_AMOSTRA.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_menu_conf_tempo_amostra(TBotao botao) {
  switch (botao) {
    case BTN_UP:
      if (menu_dec_index(&menu_cfg_tempo_amostra) >= 0) {
        est_executa_acao_do(EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA);
      }
      break;
    case BTN_DOWN:
      if ( menu_inc_index(&menu_cfg_tempo_amostra) >= 0 ) {
        est_executa_acao_do(EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA);
      }
      break;
    case BTN_STOP:
      menu_restore_index(&menu_cfg_tempo_amostra);
      est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
      break;
    case BTN_START:
      menu_confirma_index(&menu_cfg_tempo_amostra);
      serv_adcon_set_tempo_amostra_atual(menu_get_value_active(&menu_cfg_tempo_amostra));
      est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
      break;
    default:
      break;
  }//switch
   
}//est_estado_menu_conf_tempo_amostra()

/**
 * Funcao que trata as transi��es do estado EST_MENU_CONF_QUANT_SENSORES.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_menu_conf_quant_sensores(TBotao botao) {
    switch (botao) {
      case BTN_UP:
        if (menu_dec_index(&menu_cfg_quant_sensores) >= 0) {
          est_executa_acao_do(EST_ESTADO_MENU_CONF_QUANT_SENSORES);
        }
        break;
      case BTN_DOWN:
        if ( menu_inc_index(&menu_cfg_quant_sensores) >= 0 ) {
          est_executa_acao_do(EST_ESTADO_MENU_CONF_QUANT_SENSORES);
        }
        break;
      case BTN_STOP:
        menu_restore_index(&menu_cfg_quant_sensores);
        est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
        break;
      case BTN_START:
        menu_confirma_index(&menu_cfg_quant_sensores);
        serv_adcon_set_quant_sensores_atual((uint8_t)(menu_get_value_active(&menu_cfg_quant_sensores)));
        est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
        break;
      default:
        break;
    }//switch
}//est_estado_menu_conf_quant_sensores()

/**
 * Funcao que trata as transi��es do estado EST_ESTADO_ENVIAR_DADOS.
 * @param botao o bot�o que foi pressionado.
 */
static void est_estado_enviar_dados(TBotao botao) {
  if (botao == BTN_STOP) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_enviar_dados()

/**
 * Executa a a��o "do" dos estados.
 * o par�metro novo_estado � o estado que ser� executada a a��o "do".
 * Atualiza a variavel est_estado_atual = novo_estado.
 * @param novo_estado o proximo estado para a m�quina de estados.
 */
static void est_executa_acao_do(TEstado novo_estado) {
  uint8_t indice_menu;

  switch (novo_estado) {
      
    case EST_ESTADO_MENU_PRINCIPAL:
      //Limpa o LCD e imprime o novo item de menu principal.
      lcd_clear();
      indice_menu = menu_get_index_nav(&menu_principal);
      lcd_puts(menu_principal.itens[indice_menu].str_text);
      break; //EST_MENU_PRINCIPAL
      
    case EST_ESTADO_MONITORA:
      //Habilita a interrup��o do Timer0 para iniciar o monitoramento.
      INTCONbits.T0IE = 1;
      break; //EST_CAPTURA_E_MOSTRA
      
    case EST_ESTADO_MONITORA_GRAVA:
      //Habilita a interrup��o do Timer0 para iniciar o monitoramento.
      INTCONbits.T0IE = 1;
      break; //EST_CAPTURA_E_GRAVA
      
    case EST_ESTADO_MOSTRA_TODOS:
      //A cada entrada neste estado mostra a pr�xima amostra armazenada na EEPROM.
      serv_adcon_print_amostra_eeprom(est_mostra_todos_index_amostra);
      break; //EST_MOSTRA_TODOS
    
    case EST_ESTADO_MOSTRA_MAX_MIN:
      //Mostra o valor m�ximo e m�nimo armazenados na EEPROM.
      serv_adcon_print_max_min();
      break; //EST_MOSTRA_MAX_MIN
      
    case EST_ESTADO_LIMPAR:
      lcd_clear();
      lcd_puts("Limpando...");
      //Inicializa toda a EEPROM com os valores default.
      serv_eeprom_inicializa_configuracoes();
      serv_eeprom_limpa_dados();
      lcd_clear();
      lcd_puts("Tudo Limpo.");
      break; //EST_LIMPAR
      
    case EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA:
      //Atualiza o menu.
      indice_menu = menu_get_index_nav(&menu_cfg_tempo_amostra);
      lcd_clear();
      lcd_puts(menu_cfg_tempo_amostra.itens[indice_menu].str_text);
      break; //EST_MENU_CONF_TEMPO_AMOSTRA

    case EST_ESTADO_MENU_CONF_QUANT_SENSORES:
      //Atualiza o menu.
      lcd_clear();
      indice_menu = menu_get_index_nav(&menu_cfg_quant_sensores);
      lcd_puts(menu_cfg_quant_sensores.itens[indice_menu].str_text);
      break; //EST_MENU_CONF_QUANT_SENSORES
      
      #ifdef _ENVIA_DADOS_SERIAL_
      case EST_ESTADO_ENVIAR_DADOS:
        serv_rs232_envia_leituras_gravadas_eeprom();
        break; //EST_ENVIAR_DADOS
      #endif

    default:
      break;

    }//switch (novo_estado)
  
    //Atualiza o novo Estado.
    est_estado_atual = novo_estado;
    
}//est_executa_acao_do()
