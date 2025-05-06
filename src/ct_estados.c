/*****************************************************************************
 * File:   ct_estados.c
 * Author: alexdg
 * Comments:
 * 
 * Revision history: 
 * Created on 30 de Setembro de 2023, 11:04 *  
 *
 * Este módulo implementa a máquina de estados para o menu da interface do usuário.
 * 
 * A máquina responde a dois tipos de ventos
 * - Pressionamento de um botão da interface do equipamento.
 * - Overflow do Timer0, normalmente utilizando para gerar os tempos durante uma sessão de monitoramento.
 * - RS232: para responder ao protocolo da RS232.
 * 
 * Ao ser pressionado um botão, a seguinte sequência de chamadas de funções é executada:
 * 1) Chama a função est_maquina(botao) que verifica o estado atual e 
 * 2) chama a função específica (est_testa_estado_nome_estado) para tratar as transições do estado e a execução da ação de transição,
 * 3) chama a função específica para executar a ação "entry" do novo estado (est_entra_estado_nome_estado).
 * 4) Atualiza a variável est_estado_atual para representar o novo estado da máquina.
  *  
 * Detalhamento:
 * 
 * 1)
 * est_maquina(TBotao) - é a funcao de entrada da máquina de estados.
 * Toda transição de estado começa por esta função, que é chamada pela interrupção que foi gerada pelo botão.
 * O botão que foi pressionado é passado como parâmetro para a função.
 * Esta função faz 3 ações:
 * 1.1) Chama a função específica para tratar a transição de estado (est_testa_estado_nome_do_estado).
 * 1.2) Chama a função específica para executar a ação "entry" do novo estado (est_entra_estado_nome_estado).
 * 1.3) Atualiza o estado da máquina (atualiza a variável est_estado_atual).
 * 
 * 1.1)
 * Funções específicas de tratamento de cada estado:
 * Cada estado possui uma função específica que executa as ações de transição que ocorrem no estado.
 * As ações executadas durante uma transição estão implementadas nessas funções específicas.
 * Por exemplo: a ação "decrementar item menu" ao clicar no botão UP enquanto no estado EST_MENU_CONF_QUANT_SENSORES
 * está implementada na função est_testa_menu_conf_quant_sensores().
 * 
 * As funções específicas de tratamento de cada estado são listadas a seguir:
 * est_testa_estado_inicial(TBotao botao);
 * ...
 * est_testa_estado_monitora(TBotao botao);
 * est_testa_estado_monitora_grava(TBotao botao);
 * est_testa_estado_mostra_todos(TBotao botao);
 * est_testa_estado_mostra_max_min(TBotao botao);
 * est_testa_estado_limpar(TBotao botao);
 * est_testa_estado_enviar_dados(TBotao botao);
 * ...
 *  
 * 1.2)
 * Testa a variável est_estado_novo, caso ela seja diferente de NULL então chama a função específica
 * para executar a ação "entry" do novo estado. A função "entry" de cada estado deve atualizar
 * a variável est_estado_novo, que será utilizada pela função est_maquina().
 * Exemplo de função de entrada de um estado: est_entra_estado_menu_conf_tempo_aquisicao()
 * 
 * 1.3) Atualiza a variável est_estado_atual = est_estado_novo.
 *  
 * ATENÇÂO:
 * Quanto acontece uma auto-transição a máquina sai do estado atual e entra novamente no mesmo estado.
 *  
 * ===============================================================
 * 
 * Exemplo para adiconar um novo item no Menu Principal, chamado MENU_NOVO.
 * As seguintes ações são necessárias:
 * 1) Em serv_menus.h
 *    1.1) Ajustar MENU_PRINCIPAL_TAM
 *    1.2) Incluir o item "Menu Novo" em menu_principal_itens[]
 *    1.3) Se o item também for um Menu, então:
 *         1.3.1)Criar uma macro MENU_NOVO_TAM
 *         1.3.2)Criar const TMenuItem menu_novo_itens[MENU_NOVO_TAM]
 *         1.3.3)Criar variavel TMenu menu_novo;
 * 
 * 
 * Para adicionar um novo estado na máquina de estados:
 * 1) Em ct_estados.h adicionar o novo ESTADO no tipo TEstado (exemplo EST_ESTADO_MENU_NOVO).
 * 3) Em ct_estados.c
 *    3.1) Implementar a funcao static void est_trata_estado_menu_novo(TBotao botao);
 *    3.2) Implementar a funcao static void est_entra_estado_menu_novo();
 *    3.2) Adicionar dois cases, um em cada um dos dois switch da funcao est_maquina(), 
 *         chamando as funções est_trata_estado_menu_novo() e est_entra_estado_menu_novo(), respectivamente.
 *    3.3) Na função est_trata_estado_menu_principal() adicionar um sub-case ao case BTN_START fazendo est_estado_novo = EST_ESTADO_MENU_NOVO.
 * 
 */

//===== Includes =============================================================

#include <xc.h>
#include <stdio.h>

#include "xtal.h"
#include "ct_estados.h"
//#include "ct_handler.h"
#include "versao.h"
//#include "base_botoes.h" //Já incluido no .h.
#include "base_lcd.h"
#include "serv_adcon.h"
#include "serv_eeprom.h"
#include "serv_menus.h"
#include "base_rs232.h"
#include "serv_dht22.h"

//============================================================================
//===== Definições Públicas ==================================================
//============================================================================

//===== Constantes Públicas ==================================================

//===== Variaveis Públicas ===================================================

//Variável global que matem o estado atual da máquina de estados.  
//Ao ligar o equipamento ele estará no ESTADO_NULL e passará em seguida para ESTADO_INICIAL.
TEstado est_estado_atual = EST_ESTADO_NULL;

//============================================================================
//===== Definições e Declaraçoes Privadas ====================================
//============================================================================

//===== Constantes Privadas ==================================================

//===== Tipos Privados =======================================================

//===== Variáveis Privadas ===================================================

//Variável local para o novo estado, sendo atribuída nas funções específicas
//de tratamento de cada estado. Se a função est_maquina() detectar que
//houve transição de estado, ou seja, se est_estado_novo != EST_ESTADO_NULL,
//então será feita a atualização est_estado_atual = est_estado_novo e chamada
//a função est_entra_estado_novo().
static TEstado est_estado_novo  = EST_ESTADO_NULL;

//Mantem o indice da aquisição que está sendo mostrada no display quando
//estiver no estado EST_ESTADO_VER_AQUISICOES
static uint8_t est_ver_aquisicoes_index = 0;

//Indica se o equipamento foi inicializado. Ao ligar o equipamento, 
//após inicializar o mesmo, esta variável é colocada em 1.
//Atualmente esta variável não está sendo utilizada.
static uint8_t est_equipamento_inicializado = 0;

//===== Declaração das Funções Privadas ======================================

static TMenuItem* est_navega_menus(TBotao botao);

//Funções específicas de cada estado: testa e executa as ações de transição de estado.
static void est_testa_estado_null(TBotao botao);
static void est_testa_estado_inicial(TBotao botao);
static void est_testa_estado_trata_menus(TBotao botao);
static void est_testa_estado_monitora(TBotao botao);
static void est_testa_estado_monitora_grava(TBotao botao);
static void est_testa_estado_ver_aquisicoes(TBotao botao);
static void est_testa_estado_mostra_max_min(TBotao botao);
static void est_testa_estado_limpar(TBotao botao);
static void est_testa_estado_enviar_dados(TBotao botao);

//Funções "entry" de cada estado: executa as ações de entrada do estado.
static void est_entra_estado_inicial(void);
static void est_entra_estado_trata_menus(void);
static void est_entra_estado_monitora(void);
static void est_entra_estado_monitora_grava(void);
static void est_entra_estado_mostra_max_min(void);
static void est_entra_estado_ver_aquisicoes(void);
static void est_entra_estado_limpar(void);
static void est_entra_estado_enviar_dados(void);

//Funções chamadas por est_testa_estado_trata_menus()
static void est_btn_enter_item_submenu(void);
static void est_btn_enter_item_cfg(void);
static void est_btn_enter_item_acao(void);

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

/**
 * Esta é uma das funções de entrada para execução da máquina de estados.
 * Esta função executa um "step" (passo) na máquina de estados. 
 * Caso ocorra uma transição de estado ou transição para o mesmo estado atual, 
 * então a variável est_estado_novo conterá o Estado a ser executado nesse "step". 
 * Caso contrário, a variável est_estado_novo conterá o valor EST_ESTADO_NULL,
 * indicando que não houve nenhuma transição de estado (nem no próprio estado atual).
 * A sequência de ações desta função é:
 * 1) Inicializar est_estado_novo = EST_ESTADO_NULL;
 * 2) Verifica o estado atual da máquina e chama a função específica 
 * para fazer o tratamento de transição do estado (est_testa_estado...).
 * 3) Se houve transição de estado (ou para o mesmo estado) então chama 
 * a função específica para executar a ação "entry" do novo estado (est_entra_estado...).
 * 4) Atualiza a variavel est_estado_atual para o novo estado.
 * @param botao eh o botao do equipamento que foi pressionado.
 */
void est_evento_botao(TBotao botao) {

  //Caso a variavel est_estado_novo permaneça EST_ESTADO_NULL então
  //significa que não houve transição.
  //Isso é importante quando ocorre uma auto-transição.
  //Quanto acontece uma auto-transição a máquina sai do estado atual e entra novamente no mesmo estado.
  //Para implementar esse comportamento, a estratégia foi utilizar a variável auxiliar 
  //est_estado_novo e atribuir EST_ESTADO_NULL a ela.
  est_estado_novo = EST_ESTADO_NULL;
  
  switch (est_estado_atual) {
        //case EST_ESTADO_NULL:
             //est_testa_estado_null(botao);
        break;//EST_NULL
        
        case EST_ESTADO_INICIAL:
             est_testa_estado_inicial(botao);
        break;//EST_INICIO
        
        case EST_ESTADO_TRATA_MENUS:
             est_testa_estado_trata_menus(botao);
        break;//EST_NAVEGA_MENUS
        
        case EST_ESTADO_MONITORA:
            est_testa_estado_monitora(botao);
        break; //EST_CAPTURA_E_MOSTRA

        case EST_ESTADO_MONITORA_GRAVA:
             est_testa_estado_monitora_grava(botao);
        break; //EST_CAPTURA_E_GRAVA

        case EST_ESTADO_VER_AQUISICOES:
            est_testa_estado_ver_aquisicoes(botao);
        break; //EST_MOSTRA_TODOS

        case EST_ESTADO_MOSTRA_MAX_MIN:
            est_testa_estado_mostra_max_min(botao);
        break; //EST_MOSTRA_MAX_MIN

        case EST_ESTADO_LIMPAR:
            est_testa_estado_limpar(botao);
        break; //EST_LIMPAR

        case EST_ESTADO_ENVIAR_DADOS:
            est_testa_estado_enviar_dados(botao);
        break; //EST_ENVIAR_DADOS

        default:
            break;

  }//switch (est_estado_atual)

  if (est_estado_novo != EST_ESTADO_NULL) {
    
    switch (est_estado_novo) {
      //Não precisa testar este estado.
      //case EST_ESTADO_NULL:
        //Não precisa executar nada.
        //break;

      //Não precisa, pois já está sendo tratado em est_evento_timer0().
      //case EST_ESTADO_INICIAL:
        //est_entra_estado_inicial();
        //break;
        
      case EST_ESTADO_TRATA_MENUS:
        est_entra_estado_trata_menus();
        break;
        
      case EST_ESTADO_MONITORA:
        est_entra_estado_monitora();
        break;
        
      case EST_ESTADO_MONITORA_GRAVA:
        est_entra_estado_monitora_grava();
        break;
        
      case EST_ESTADO_VER_AQUISICOES:
        est_entra_estado_ver_aquisicoes();
        break;
        
      case EST_ESTADO_MOSTRA_MAX_MIN:
        est_entra_estado_mostra_max_min();
        break;

      case EST_ESTADO_LIMPAR:
        est_entra_estado_limpar();
        break; //EST_LIMPAR

      case EST_ESTADO_ENVIAR_DADOS:
        //est_entra_estado_enviar_dados();
        break;
        
      default:
        break;
      
    }//switch (est_estado_novo)
    
    //Atualiza o estado atual.
    est_estado_atual = est_estado_novo;

  }//if
  
}//est_evento_botao()

//Esta função deve ser chamada quando acontecer um evento do Timer0.
void est_evento_timer0() {
      static uint16_t static_count_t0 = 0;
      
      
      //Se recem ligou o equipamento, então faz inicializações e troca est_estado_atual para EST_ESTADO_INICIAL.
      if (est_estado_atual == EST_ESTADO_NULL) {
        est_estado_atual = EST_ESTADO_INICIAL;
        est_entra_estado_inicial();
        //est_maquina(BTN_NULL);
      }
      
      #ifdef _HARDWARE_2013_
      //Trata interface dos botões.
      //Só vai funcionar se o Timer0 estiver ligado, não deveria ser assim.
      TBotao botao;
      botao = btns_testa_antigo();
      if (botao != 0) {
        est_evento_botao(botao);
      }
      #endif

      //Trata leitura sensores.
      else if (  (est_estado_atual==EST_ESTADO_MONITORA) ||  (est_estado_atual==EST_ESTADO_MONITORA_GRAVA)  ) {
        uint16_t tempo;
        //Verifica no timer0 se já passou a contagem de tempo para efetuar uma amostra.
        tempo = serv_adcon_testa_contagem_timer(static_count_t0);
        if (tempo) {
          //Ja passou a contagem do Timer0, então efetua uma amostra e zera a contagem.
          if (est_estado_atual == EST_ESTADO_MONITORA) {
            serv_adcon_aquisicao_amostrar();
            serv_adcon_aquisicao_imprimir(adcon_cfg_quant_sensores_atual);
            serv_dht22_amostra_e_print();
            //Não precisa (nem pode) desabilitar a interrupção global aqui
            //pois o próprio handler já desabilita automaticamente.
          } else if (est_estado_atual==EST_ESTADO_MONITORA_GRAVA ) {
            //Se o módulo serv_adcon está monitorando e gravando então
            if (serv_adcon_bol_monitora_grava) {
              serv_adcon_aquisicao_amostrar_imprimir_gravar();
            }
            else {
              //Módulo sinalizou que interrompeu a gravação, então deveria avisar máquina de estados e não atribuir diretamente, verificar<<<<<<<<<<<<<<<<<<
              //<<<<<<<<<<<<<<<<<<<
              //serv_adcon_monitora_grava = 0;  //<<<<<<<< função serv_adcon_aquisicao_print_grava() já fez isso. retirado em 2024-08-03 alexdg)
            }
          }
          static_count_t0 = 0;
        } 
        else {
          static_count_t0++;
        }//else
      }//if est_estado_atual

      //TMR0 = 39; //para dar overflow antes de 256 ints
      //INTCONbits.T0IE = 1;

}//est_evento_timer0

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

//===========================================================================
//================== Funções Testa_Estado... ================================
//===========================================================================

/**
 * Estado ao ligar a máquina, que passará automaticamente para o ESTADO_INICIAL.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_null(TBotao botao) {
  est_estado_novo = EST_ESTADO_INICIAL;
}//est_testa_estado_null()

/**
 * Estado inicial ao ligar a máquina. Ao pressionar qualquer botão irá para EST_ESTADO_MENU_PRINCIPAL.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_inicial(TBotao botao) {
  
  //Qualquer botão vai para o estado EST_ESTADO_MENU_PRINCIPAL.
  if (botao==BTN_START || botao==BTN_DOWN || botao==BTN_UP ) {
    est_estado_novo = EST_ESTADO_TRATA_MENUS;
    //est_estado_novo = EST_ESTADO_TRATA_MENU; //<<<<<< fazer retirar
  }
  
}//est_testa_estado_inicial()

//===========================================================================
//================== Funções Testa_Estado (Menus) ===========================
//===========================================================================

/* Faz o tratamento do sistema de menus.
 * Botões UP, DOWN e ESC: somente atualiza os menus e continua no estado EST_ESTADO_TRATA_MENUS.
 * Botão ENTER:  Chama uma das seguintes funções:
 * est_btn_enter_item_submenu().
 * est_btn_enter_item_cfg().
 * est_btn_enter_item_acao().
 */
static void est_testa_estado_trata_menus(TBotao botao) {
  uint8_t item_is_submenu;
  TMnuTipoMenu tipo_menu;
  
  switch (botao) {
    
    case BTN_UP:
      //est_navega_menus(botao);
      mnu_exec_up(&serv_menu_raiz);
      est_estado_novo = EST_ESTADO_TRATA_MENUS;
      break;
      
    case BTN_DOWN:
      //est_navega_menus(botao);
      mnu_exec_down(&serv_menu_raiz);
      est_estado_novo = EST_ESTADO_TRATA_MENUS;
      break;
      
    case BTN_STOP:
      //est_navega_menus(botao);
      mnu_exec_esc(&serv_menu_raiz);
      est_estado_novo = EST_ESTADO_TRATA_MENUS;
      break;
      
    case BTN_START:
      //Verifica se o item de menu é um submenu.
      item_is_submenu = mnu_item_nav_is_submenu(&serv_menu_raiz);
      if (item_is_submenu) {
        est_btn_enter_item_submenu();
        est_estado_novo = EST_ESTADO_TRATA_MENUS;
      }//item is submenu
      else  { //Não é um submenu.
        //tipo_menu = mnu_item_nav_get_tipo(&serv_menu_raiz);
        tipo_menu = mnu_menu_atual_get_tipo(&serv_menu_raiz);
        if (tipo_menu == MNU_TIPO_MENU_CFG_LISTA) {
          //=============== Executa ação do item de configuração ============
          est_btn_enter_item_cfg();
          est_estado_novo = EST_ESTADO_TRATA_MENUS;
        }//MNU_TIPO_MENU_CFG_LIST
        else {
          //=============== Executa ação do item de ação ============================
          est_btn_enter_item_acao();
        }//else tipo_menu == MNU_TIPO_MENU_CFG_LIST
      }//else item_is_submenu
      
    case BTN_NULL:
      break;
    
  }//switch(botao)
  
}//est_testa_estado_trata_menus

/* Trata o evento botão ENTER sobre um item de submenu.
 * Se o novo submenu for um submenu de configuração, então precisa setar o item ativo do submenu
 * baseado no valor de configuração do sistema (pois pode ter sido modificado assincronamente, por exemplo, pela RS232).
 */
static void est_btn_enter_item_submenu(void) {
  TMnuTipoMenu tipo_menu;
  //Entra no submenu e verifica se é um submenu de configuração,
  //se for o caso, então precisa ativar o item correto desse menu pois as variáveis globais de configuração podem
  //ter sido modificadas assincronamente (por exemplo, por um comando da porta serial).
  //Então o item ativo do menu deve refletir essa modificação.
  mnu_exec_enter(&serv_menu_raiz);
  tipo_menu = mnu_menu_atual_get_tipo(&serv_menu_raiz);
  if (tipo_menu == MNU_TIPO_MENU_CFG_LISTA) {
    //Pega o item navegado para saber se é uma ação de configuração e qual a ação.
    uint8_t codigo_acao = mnu_item_nav_get_cod_acao(&serv_menu_raiz);
    if (codigo_acao == SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO) {
      mnu_menu_atual_set_item_ativo_from_value(&serv_menu_raiz, adcon_cfg_tempo_aquisicao_atual);
    }
    else if (codigo_acao == SERV_MENU_ACAO_CFG_QUANT_SENSORES) {
      mnu_menu_atual_set_item_ativo_from_value(&serv_menu_raiz, adcon_cfg_quant_sensores_atual);
    }
  }//MNU_TIPO_MENU_CFG_LIST
}//est_btn_enter_item_submenu())

  
/* Trata o evento botão ENTER sobre um item de configuração, ou seja,
 * efetua a configuração.
 */
static void est_btn_enter_item_cfg(void) {
  TServMenuCodigoAcaoCfg  acao_cfg;
  uint16_t valor;
  mnu_exec_enter(&serv_menu_raiz);
  acao_cfg = mnu_item_executado_get_cod_acao(&serv_menu_raiz);
  valor = mnu_item_executado_get_value(&serv_menu_raiz);

  //Tem que executar ESC para deixar o submenu atual.
  mnu_exec_esc(&serv_menu_raiz);

  switch(acao_cfg){
    case SERV_MENU_ACAO_CFG_QUANT_SENSORES:
      serv_adcon_set_quant_sensores_atual((uint8_t)valor);
      break;

    case SERV_MENU_ACAO_CFG_TEMPO_AQUISICAO:
      serv_adcon_set_tempo_aquisicao_atual(valor);
      break;

    default:
      break;
  }//switch(acao)
}//est_btn_enter_item_cfg())

/* Trata o evento botão ENTER sobre um item de ação, ou seja,
 * coloca a máquina de estados no novo estado.
 */
static void est_btn_enter_item_acao(void) {
  TServMenuCodigoAcaoExec acao_exec;
  mnu_exec_enter(&serv_menu_raiz);
  acao_exec = mnu_item_executado_get_cod_acao(&serv_menu_raiz);
      
  switch(acao_exec){
    case SERV_MENU_ACAO_EXEC_MONITORA:
      est_estado_novo = EST_ESTADO_MONITORA;
      //TMR0 = 39;
      //INTCONbits.T0IE = 1; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      break;
          
    case SERV_MENU_ACAO_EXEC_MONITORA_GRAVA:
      est_estado_novo = EST_ESTADO_MONITORA_GRAVA;
      //TMR0 = 39;
      //INTCONbits.T0IE = 1; //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      break;

    case SERV_MENU_ACAO_EXEC_VER_AQUISICOES:
      est_estado_novo = EST_ESTADO_VER_AQUISICOES;
      break;

    case SERV_MENU_ACAO_EXEC_MOSTRA_MAX_MIN:
      est_estado_novo = EST_ESTADO_MOSTRA_MAX_MIN;
      break;

    case SERV_MENU_ACAO_EXEC_LIMPAR:
      est_estado_novo = EST_ESTADO_LIMPAR;
      break;

    case SERV_MENU_ACAO_EXEC_ENVIAR_DADOS:
      est_estado_novo = EST_ESTADO_ENVIAR_DADOS;
      break;

    default:
      break;
  }//switch(acao)
}//est_btn_enter_item_acao()

//===========================================================================
//================== Funções Testa_Estado (Ações) ===========================
//===========================================================================

/* Funcao que trata as transições do estado EST_ESTADO_MONITORA.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_monitora(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_TRATA_MENUS;
  }
}//est_testa_estado_monitora()

/**
 * Funcao que trata as transições do estado EST_MONITORA_GRAVA.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_monitora_grava(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_TRATA_MENUS;
  }
}//est_testa_estado_monitora()

/**
 * Funcao que trata as transições do estado EST_ESTADO_VER_AQUISICOES.
 * Controla a navegacao por todos os valores ja gravados na EEPROM.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_ver_aquisicoes(TBotao botao) {
  switch (botao) {
    case BTN_UP:
      if (est_ver_aquisicoes_index > 0) {
        est_ver_aquisicoes_index--;
        est_estado_novo = EST_ESTADO_VER_AQUISICOES;
      }
      break;
    case BTN_DOWN:
      if (serv_adcon_aquisicao_testa_indice_valido(est_ver_aquisicoes_index)) {
        est_ver_aquisicoes_index++;
        est_estado_novo = EST_ESTADO_VER_AQUISICOES;
      }
      break;
    case BTN_STOP:
      est_estado_novo = EST_ESTADO_TRATA_MENUS;
      break;
    default:
      break;
  }//switch
}//est_testa_estado_mostra_todos()

/**
 * Funcao que trata as transições do estado EST_MOSTRA_MAX_MIN.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_mostra_max_min(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_TRATA_MENUS;
  }
}//est_testa_estado_mostra_max_min()


/**
 * Funcao que trata as transições do estado EST_MENU_LIMPAR.
  * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_limpar(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_TRATA_MENUS;
  }
}//est_testa_estado_limpar())

/**
 * Funcao que trata as transições do estado EST_ESTADO_ENVIAR_DADOS.
 * @param botao o botão que foi pressionado.
 */
static void est_testa_estado_enviar_dados(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_TRATA_MENUS;
  }
}//est_testa_estado_enviar_dados()

//===========================================================================
//================== Funções Entra_Estado... ================================
//===========================================================================

/**/
static void est_entra_estado_inicial(void) {
    //rs232_init() precisa ser antes do printf() e antes do lcd_puts() pois 
    //em modo __DEUBG essas funções utilizam a UART.      
    rs232_init();
    
    lcd_init();    

    lcd_puts(_VER_STR_VERSAO_ _VER_STR_HARDWARE_NOVO_ANTIGO_);
    lcd_goto(2, 0);
    lcd_puts("Iniciando...");
    
    //<<<< verificar >>> Tem que ser antes de habilitar teclado e depois da mensagem na tela
    //para garantir mostrar mensagem na tela.
    btns_init();

    // the Power-up Timer (72 ms duration) prevents EEPROM write:
    // 140ms = tempo maximo do power-up:
    __delay_ms(200);

    //Testa se a EEPROM já foi gravada pelo equipamento.
    //Se ainda não foi, então inicializa com os valores de configuração padrão.
    if (!serv_eeprom_verifica_inicializacao()) {
      serv_adcon_eeprom_inicializa_configuracoes();
      serv_adcon_eeprom_limpa_dados();
    }
    else {
      //Chave de inicialização existe, então le as configurações da EEPROM.
      serv_adcon_eeprom_le_configuracoes();
    }

    //Inicializa menus.
    serv_menu_princ_inicializa();
  
    //Atualiza menus conforme valores da eeprom.
    //menu_set_value_indexes(&menu_cfg_quant_sensores, adcon_cfg_quant_sensores_atual);
    //menu_set_value_indexes(&menu_cfg_tempo_aquisicao,  adcon_cfg_tempo_aquisicao_atual);
    //menu_set_value_indexes(&menu_configuracoes,  1);
    
    lcd_goto(2, 0);
    lcd_puts("Pronto!         ");
    
    est_equipamento_inicializado = 1;

}//est_entra_estado_inicial()

/*
 */
static void est_entra_estado_trata_menus(void) {
  //Atualizar Menu:
  
  const char* titulo;
  const char* texto;
  lcd_clear();
  //titulo = serv_menu_raiz.p_menu_atual->str_titulo;
  titulo = mnu_menu_atual_get_titulo(&serv_menu_raiz);
  lcd_puts(titulo); 
  lcd_goto(2,0);
  texto = mnu_item_nav_get_text(&serv_menu_raiz);
  lcd_puts(texto); 
  
}//est_entra_estado_trata_menu()

/*
 */
static void est_entra_estado_monitora(void) {
  //Desabilita as interrupções.
  //Por que? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  INTCONbits.GIE = 0;
  
  //serv_adcon_aquisicao_print();
  serv_adcon_aquisicao_amostrar();
  serv_adcon_aquisicao_imprimir(adcon_cfg_quant_sensores_atual);
  serv_dht22_amostra_e_print();
  //Habilita as interrupções.
  INTCONbits.GIE = 1;
}//est_entra_estado_monitora()

/*
 */
static void est_entra_estado_monitora_grava(void) {
  //Desabilita as interrupções.
  //Por que? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  INTCONbits.GIE = 0;

  //Marca que o monitramento será iniciado.
  serv_adcon_bol_monitora_grava = 1;
  //Faz uma amostragem.
  serv_adcon_aquisicao_amostrar_imprimir_gravar();
  
  //Habilita as interrupções.
  INTCONbits.GIE = 1;
}//est_entra_estado_monitora_grava()

/*
 */
static void est_entra_estado_mostra_max_min(void) {
      serv_adcon_print_max_min();
}//est_entra_estado_mostra_max_min()

/*
 */
static void est_entra_estado_ver_aquisicoes(void) {
  serv_adcon_aquisicao_imprimir_da_eeprom(est_ver_aquisicoes_index);
}//est_entra_estado_ver_aquisicoes()

/*
 */
static void est_entra_estado_limpar(void) {
  //Recém entrou no estado.
  lcd_clear();
  lcd_puts("Limpando...");
  //Inicializa toda a EEPROM com os valores default.
  serv_adcon_eeprom_inicializa_configuracoes();
  serv_adcon_eeprom_limpa_dados();
  lcd_clear();
  lcd_puts("Tudo Limpo.");
}//est_entra_estado_limpar()

/*
 */
static void est_entra_estado_enviar_dados(void) {
  serv_adcon_rs232_envia_amostras_gravadas_eeprom();
}//est_entra_estado_enviar_dados()

//===== Final do Arquivo =====================================================
