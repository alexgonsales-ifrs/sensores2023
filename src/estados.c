/*
 * Este módulo implementa a máquina de estados para o menu da interface do usuário.
 * 
 * Ao ser pressionado um botão, a seguinte sequência de chamadas de funções é executada:
 * 1) Chama a função est_maquina(botao) que verifica o estado atual e 
 * 2) chama a função específica do estado, que executa a ação de saída de transição do estado e
 * 3) chama a função est_executa(novo_estado), que executa a ação de entrada do novo estado e troca o estado.
 *  
 * est_maquina(TBotao) - é a funcao de entrada da máquina de estados.
 * Toda transição de estado começa por esta função, que é chamada pela interrupção que foi gerada pelo botão.
 * O botão que foi pressionado é passado como parâmetro para a função.
 * Esta função somente verifica o estado atual da máquina de estados e chama a função específica para tratar o estado,
 * passando adiante o botão que foi pressionado.
 * 
 * Funções específicas de cada estado:
 * Cada estado possui uma função específica que executa as ações de transição que ocorrem no estado.
 * As ações executadas durante uma transição estão implementadas nessas funções específicas (origem da transição).
 * Por exemplo: a ação "decrementar item menu" ao clicar no botão UP enquanto no estado EST_CONF_QUANT_SENSORES
 * está implementada na função est_estado_menu_conf_quant_sensores().
 * 
 * As ações específicas de cada estado* são listadas a seguir:
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
 * Esta função executa as ações "do" (fazer) de cada estado e 
 * realiza também a troca para o novo_estado.
 *  
 */

#include <xc.h>
#include <stdio.h>

#include "estados.h"
#include "versao.h"
#include "adcon.h"
#include "botoes.h"
#include "eeprom.h"
#include "lcd.h"
#include "menu_principal.h"
#include "cfg_quant_sensores.h"
#include "cfg_tempo_amostra.h"
#include "rs232.h"
#include "servicos.h"

/*********************************************************************
******************** Definições Públicas do Módulo *******************
*********************************************************************/

//Variável global que matem o estado atual da máquina de estados.  
TEstado est_estado_atual = EST_ESTADO_INICIAL;

/*********************************************************************
**************** Definições Privadas do Módulo ***********************
*********************************************************************/

/******************** Variaveis Privadas ****************************/

//Mantem o indice da amostra que está sendo mostrada no display quando
//estiver no estado EST_ESTADO_MOSTRA_TODOS.
static uint8_t est_mostra_todos_index_amostra = 0;

/******************* Funções Privadas ******************************/

//Funções específicas de cada estado.
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

//Função que executa a ação "do" dos estados.
//o parâmetro novo_estado é o estado que será executada a ação "do".
static void est_executa_acao_do(TEstado novo_estado);

/*********************************************************************
********** Definições (implementação) das Funções Públicas **********
*********************************************************************/ 

/**
 * Esta é a funcao de entrada da máquina de estado. Toda transição de estado
 * começa por esta função.
 * Verifica o estado atual da máquina e chama a função específica do estado.
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
********* Definições (implementação) das Funções Privadas  ***********
*********************************************************************/

/**
 * A partir do estado inicial muda para o estado EST_MENU_PRINCIPAL
 * @param botao o botão que foi pressionado.
 */
static void est_estado_inicial(TBotao botao) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
}//est_estado_inicial()

/**
 * Funcao que trata as transições do estado EST_MENU_PRINCIPAL.
 * @param botao o botão que foi pressionado.
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
          est_executa_acao_do(EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA);
          break;

        case 6: //"Quant Sensores"
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
 * Funcao que trata as transições do estado EST_MENU_LIMPAR.
  * @param botao o botão que foi pressionado.
 */
static void est_estado_limpar(TBotao botao) {
  if (botao == BTN_STOP) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_menu_limpar()

/* Funcao que trata as transições do estado EST_MONITORA.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_monitora(TBotao botao) {
  if (botao == BTN_STOP) {
    INTCONbits.T0IE = 0;
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_monitora()

/**
 * Funcao que trata as transições do estado EST_MONITORA_GRAVA.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_monitora_grava(TBotao botao) {
  if (botao == BTN_STOP) {
    INTCONbits.T0IE = 0;
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_monitora()

/**
 * Funcao que trata as transições do estado EST_ESTADO_MOSTRA_TODOS.
 * Controla a navegacao por todos os valores ja gravados na EEPROM.
 * @param botao o botão que foi pressionado.
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
      if (est_mostra_todos_index_amostra < adcon_quant_leituras_gravadas / cfg_quant_sensores_atual - 1) {
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
 * Funcao que trata as transições do estado EST_MOSTRA_MAX_MIN.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_mostra_max_min(TBotao botao) {
  if (botao == BTN_STOP) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_mostra_max_min()

/**
 * Funcao que trata as transições do estado EST_MENU_CONF_TEMPO_AMOSTRA.
 * @param botao o botão que foi pressionado.
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
      cfg_tempo_amostra_atual = (uint8_t)(menu_get_value_active(&menu_cfg_tempo_amostra));
      eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, cfg_tempo_amostra_atual);
      est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
      break;
    default:
      break;
  }//switch
   
}//est_estado_menu_conf_tempo_amostra()

/**
 * Funcao que trata as transições do estado EST_MENU_CONF_QUANT_SENSORES.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_menu_conf_quant_sensores(TBotao botao) {
  uint8_t quant_sens;
    switch (botao) {
      case BTN_UP:
        //vetor que guarda a quantidade de sensores inicia em 1:
        if (menu_dec_index(&menu_cfg_quant_sensores) >= 0) {
          est_executa_acao_do(EST_ESTADO_MENU_CONF_QUANT_SENSORES);
        }
        break;
      case BTN_DOWN:
        //vetor que guarda a quantidade de sensores inicia em 1:
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
        cfg_quant_sensores_atual = (uint8_t)(menu_get_value_active(&menu_cfg_quant_sensores));
        eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, cfg_quant_sensores_atual);
        est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
        break;
      default:
        break;
    }//switch
}//est_estado_menu_conf_quant_sensores()

/**
 * Funcao que trata as transições do estado EST_ESTADO_ENVIAR_DADOS.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_enviar_dados(TBotao botao) {
  if (botao == BTN_STOP) {
    est_executa_acao_do(EST_ESTADO_MENU_PRINCIPAL);
  }
}//est_estado_enviar_dados()

/**
 * Executa a ação "do" dos estados.
 * o parâmetro novo_estado é o estado que será executada a ação "do".
 * Atualiza a variavel est_estado_atual = novo_estado.
 * @param novo_estado o proximo estado para a máquina de estados.
 */
static void est_executa_acao_do(TEstado novo_estado) {
  //uint16_t maior, menor, temp_int, pos_taxa;
  uint16_t leitura_temperatura;
  uint8_t endereco;
  uint8_t index;
  //uint8_t posicao;
  #ifdef _ENVIA_DADOS_SERIAL_
    uint8_t bytes_tx;
  #endif
  char tmp[17] = {0}; //warning do compilador
    
  //pos_taxa = eeprom_ler_word(END_TX_AMOSTRA);
  switch (novo_estado) {
      
    case EST_ESTADO_MENU_PRINCIPAL:
      //Limpa o LCD e imprime o novo item de menu principal.
      lcd_clear();
      index = menu_get_index_nav(&menu_principal);
      lcd_puts(menu_principal.itens[index].str_text);
      break; //EST_MENU_PRINCIPAL
      
    case EST_ESTADO_MONITORA:
      //Habilita a interrupção do Timer0 para iniciar o monitoramento.
      INTCONbits.T0IE = 1;
      break; //EST_CAPTURA_E_MOSTRA
      
    case EST_ESTADO_MONITORA_GRAVA:
      //Habilita a interrupção do Timer0 para iniciar o monitoramento.
      INTCONbits.T0IE = 1;
      break; //EST_CAPTURA_E_GRAVA
      
    case EST_ESTADO_MOSTRA_TODOS:
      //A cada entrada neste estado mostra o próximo conjunto de dados armazenados na EEPROM.
      //Se não tiver nenhum dado, finaliza.
      lcd_clear();
      if (adcon_quant_leituras_gravadas == 0) {
        lcd_puts("Nenhum Dado");
      }
      //Caso tenha sido modificada a quantidade de sensores.
      else if (adcon_quant_leituras_gravadas < cfg_quant_sensores_atual) {
        lcd_puts("Erro qt dados");
      }
      else {
        endereco = EEPROM_END_INICIO_AMOSTRAS + (est_mostra_todos_index_amostra * cfg_quant_sensores_atual * 2);
        //Mostrar no LCD os valores de uma amostra (todos os sensores) gravada na EEPROM:
        for (uint8_t i = 0; i < cfg_quant_sensores_atual; i++) {
            leitura_temperatura = eeprom_le_word(endereco);
            sprintf(tmp, "%d=%d.%d", i + 1, leitura_temperatura / 10, leitura_temperatura % 10);
            lcd_goto_sensor(i);
            lcd_puts(tmp);
            endereco = endereco + 2; //cada leitura ocupa 2 bytes.
        }//for
      }//else
    break; //EST_MOSTRA_TODOS
    
    case EST_ESTADO_MOSTRA_MAX_MIN:
      //Mostra o valor máximo e mínimo armazenados na EEPROM.
      lcd_clear();
      if (adcon_quant_leituras_gravadas == 0) {
          lcd_puts("Nenhum Dado");
      }
      else {
        sprintf(tmp, "Max=%d.%d", adcon_leitura_max / 10, adcon_leitura_max % 10);
        lcd_puts(tmp);
        lcd_goto(2, 0);
        sprintf(tmp, "Min=%d.%d", adcon_leitura_min / 10, adcon_leitura_min % 10);
        lcd_puts(tmp);
      }//else
      break; //EST_MOSTRA_MAX_MIN
      
    case EST_ESTADO_LIMPAR:
      lcd_clear();
      lcd_puts("Limpando...");
      //Inicializa toda a EEPROM com os valores default.
      serv_eeprom_inicializa_configuracoes();
      serv_eeprom_limpa_dados();
      /*
      eeprom_write(EEPROM_END_QTDE_SENSORES_ATUAL, 1);
      eeprom_write(EEPROM_END_QTDE_LEITURAS, 0);
      eeprom_grava_word(EEPROM_END_TEMPO_AMOSTRAGEM, CFG_TEMPO_AMOSTRA_1_SEGUNDO);
      eeprom_grava_word(EEPROM_END_LEITURA_MIN, ADCON_VALOR_MAXIMO_LEITURA);
      eeprom_grava_word(EEPROM_END_LEITURA_MAX, ADCON_VALOR_MINIMO_LEITURA);
      for (uint8_t i = 0; i < ADCON_QTD_MAX_LEITURAS * 2; i++) {
        eeprom_write(i + EEPROM_END_INICIO_AMOSTRAS, 0);
      }
      */
      lcd_clear();
      lcd_puts("Tudo Limpo.");
      break; //EST_LIMPAR
      
    case EST_ESTADO_MENU_CONF_TEMPO_AMOSTRA:
      //Entra no menu Conf. Tempo Amostra.
      index = menu_get_index_nav(&menu_cfg_tempo_amostra);
      lcd_clear();
      lcd_puts(menu_cfg_tempo_amostra.itens[index].str_text);
      break; //EST_MENU_CONF_TEMPO_AMOSTRA

    case EST_ESTADO_MENU_CONF_QUANT_SENSORES:
      //Entra no menu Conf. Quant. de Sensores.
      lcd_clear();
      index = menu_get_index_nav(&menu_cfg_quant_sensores);
      lcd_puts(menu_cfg_quant_sensores.itens[index].str_text);
      break; //EST_MENU_CONF_QUANT_SENSORES
      
      #ifdef _ENVIA_DADOS_SERIAL_
      case EST_ESTADO_ENVIAR_DADOS:
        lcd_clear();
        lcd_puts("Env. serial...");
        bytes_tx = serv_rs232_envia_leituras_gravadas_eeprom();
        lcd_clear();
        sprintf(tmp, "%d bytes transmitidos", bytes_tx);
        lcd_puts(tmp);
        break; //EST_ENVIAR_DADOS
      #endif

    default:
      break;

    }//switch (novo_estado)
  
    //Atualiza o novo Estado.
    est_estado_atual = novo_estado;
    
}//est_executa_acao_do()
