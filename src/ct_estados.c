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
 * Ao ser pressionado um botão, a seguinte sequência de chamadas de funções é executada:
 * 1) Chama a função est_maquina(botao) que verifica o estado atual e 
 * 2) chama a função específica para tratar as transições do estado e a execução da ação de transição,
 * 3) chama a função est_entra_estado_novo ()para entrar no novo estado e executar a ação "entry" do novo estado.
 *  
 * Detalhamento:
 * 
 * 1)
 * est_maquina(TBotao) - é a funcao de entrada da máquina de estados.
 * Toda transição de estado começa por esta função, que é chamada pela interrupção que foi gerada pelo botão.
 * O botão que foi pressionado é passado como parâmetro para a função.
 * Esta função somente verifica o estado atual da máquina de estados e chama a função específica para tratar o estado,
 * passando adiante o botão que foi pressionado.
 * 
 * 2)
 * Funções específicas de cada estado:
 * Cada estado possui uma função específica que executa as ações de transição que ocorrem no estado.
 * As ações executadas durante uma transição estão implementadas nessas funções específicas.
 * Por exemplo: a ação "decrementar item menu" ao clicar no botão UP enquanto no estado EST_CONF_QUANT_SENSORES
 * está implementada na função est_estado_menu_conf_quant_sensores(). A função específica de tratamento do estado
 * atualiza a variável est_estado_novo, que será utilizada pela função est_executa_estado_novo().
 * 
 * As ações específicas de cada estado* são listadas a seguir:
 *  
 * est_estado_inicial(TBotao botao);
 * est_estado_menu_principal(TBotao botao);
 * est_estado_monitora(TBotao botao);
 * est_estado_monitora_grava(TBotao botao);
 * est_estado_mostra_todos(TBotao botao);
 * est_estado_mostra_max_min(TBotao botao);
 * est_estado_limpar(TBotao botao);
 * est_estado_menu_conf_tempo_aquisicao(TBotao botao);
 * est_estado_menu_conf_quant_sensores(TBotao botao);
 * est_estado_enviar_dados(TBotao botao);
 *  
 * 3)
 * A última função executada para completar o ciclo de troca do estado é a função
 * est_executa_estado_novo(), que executa a action "entry" do novo estado e atualiza
 * a variável est_estado_atual = est_estado_novo. 
 *  
 */

//===== Includes =============================================================

#include <xc.h>
#include <stdio.h>

#include "xtal.h"
#include "ct_estados.h"
#include "versao.h"
#include "base_botoes.h" //Já incluido no .h.
#include "base_lcd.h"
#include "serv_adcon.h"
#include "serv_eeprom.h"
#include "serv_menus.h"
#include "base_rs232.h"

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

//Funções específicas de cada estado.
static void est_estado_null(TBotao botao);
static void est_estado_inicial(TBotao botao);
static void est_estado_menu_principal(TBotao botao);
static void est_estado_monitora(TBotao botao);
static void est_estado_monitora_grava(TBotao botao);
static void est_estado_ver_aquisicoes(TBotao botao);
static void est_estado_mostra_max_min(TBotao botao);
static void est_estado_limpar(TBotao botao);
static void est_estado_menu_conf_tempo_aquisicao(TBotao botao);
static void est_estado_menu_conf_quant_sensores(TBotao botao);
static void est_estado_enviar_dados(TBotao botao);

//Troca est_estado_atual para est_estado_novo e executa a ação entry do novo estado.
static void est_entra_estado_novo(void);

//============================================================================
//===== Definição (implementação) das Funções Públicas =======================
//============================================================================

/**
 * Esta é a funcao de entrada da máquina de estados e que executa um "step" 
 * Na máquina de estados. Caso ocorra uma transição de estado ou transição para 
 * o mesmo estado atual, então a variável est_estado_novo conterá estado a ser executado
 * nesse "step". Caso contrário, a variável est_estado_novo conterá o valor EST_ESTADO_NULL,
 * indicando que não houve nenhuma transição de estado (nem no próprio estado atual).
 * A sequência de ações desta função é:
 * 1) Inicializar est_estado_novo = EST_ESTADO_NULL;
 * 2) Verifica o estado atual da máquina e chama a função específica 
 * para fazer o tratamento de transição do estado.
 * 3) Se houve transição de estado (ou para o mesmo estado) então chama 
 * est_entra_estado_novo() para executar a ação "entry" do novo estado e efetivar
 * a troca para o novo estado.
 * @param botao eh o botao do equipamento que foi pressionado.
 */
void est_maquina(TBotao botao) {

  //Caso a variavel est_estado_novo permaneça EST_ESTADO_NULL então
  //significa que não houve troca de estado.
  est_estado_novo = EST_ESTADO_NULL;
  
  switch (est_estado_atual) {
        case EST_ESTADO_NULL:
             est_estado_null(botao);
        break;//EST_NULL
        
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
             est_estado_monitora_grava(botao);
        break; //EST_CAPTURA_E_GRAVA

        case EST_ESTADO_VER_AQUISICOES:
            est_estado_ver_aquisicoes(botao);
        break; //EST_MOSTRA_TODOS

        case EST_ESTADO_MOSTRA_MAX_MIN:
            est_estado_mostra_max_min(botao);
        break; //EST_MOSTRA_MAX_MIN

        case EST_ESTADO_LIMPAR:
            est_estado_limpar(botao);
        break; //EST_LIMPAR

        case EST_ESTADO_MENU_CONF_TEMPO_AQUISICAO:
            est_estado_menu_conf_tempo_aquisicao(botao);
        break; //EST_MENU_CONF_TEMPO_AMOSTRA

        case EST_ESTADO_MENU_CONF_QUANT_SENSORES:
            est_estado_menu_conf_quant_sensores(botao);
        break; //EST_MENU_CONF_QUANT_SENSORES
        
        case EST_ESTADO_ENVIAR_DADOS:
            est_estado_enviar_dados(botao);
        break; //EST_ENVIAR_DADOS

        default:
            break;

  }//switch

  if (est_estado_novo != EST_ESTADO_NULL) {
    est_entra_estado_novo();    
  }
  
}//est_maquina()

//============================================================================
//===== Definição (implementação) das Funções Privadas =======================
//============================================================================

/**
 * Estado ao ligar a máquina, que passará automaticamente para o ESTADO_INICIAL.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_null(TBotao botao) {
  est_estado_novo = EST_ESTADO_INICIAL;
}

/**
 * Estado inicial ao ligar a máquina. Ao pressionar qualquer botão irá para EST_ESTADO_MENU_PRINCIPAL.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_inicial(TBotao botao) {
  
  //Qualquer botão vai para o estado EST_ESTADO_MENU_PRINCIPAL.
  if (botao==BTN_START || botao==BTN_DOWN || botao==BTN_UP ) {
    est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
  }
  
}//est_estado_inicial()

/**
 * Funcao que trata as transições do estado EST_ESTADO_MENU_PRINCIPAL.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_menu_principal(TBotao botao) {
  uint8_t index;
  
  switch (botao) {
    
    case BTN_UP:
      if (menu_dec_index(&menu_principal) != -1) {
        est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
      }
    break;

    case BTN_DOWN:
      if ( menu_inc_index(&menu_principal) != -1 ) {
        est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
      }
    break;

    case BTN_START:
      index = menu_get_index_nav(&menu_principal);
      menu_confirma_index(&menu_principal);
      switch (index) {
        case 0: //"Monitora"
           est_estado_novo = EST_ESTADO_MONITORA;
          break;

        case 1: //"Monitora/Grava"
           est_estado_novo = EST_ESTADO_MONITORA_GRAVA;
          break;

        case 2: //"Ver Todos"
          est_estado_novo = EST_ESTADO_VER_AQUISICOES;
          break;

        case 3: //"Max/Min"
          est_estado_novo = EST_ESTADO_MOSTRA_MAX_MIN;
          
          break;

        case 4: //"Limpar"
          est_estado_novo = EST_ESTADO_LIMPAR;
          break;

        case 5: //"Tempo Amostra"
          //Seta o item do sub-menu antes entrar no sub-menu.
          menu_set_value_indexes(&menu_cfg_quant_sensores, adcon_cfg_tempo_aquisicao_atual);
          est_estado_novo = EST_ESTADO_MENU_CONF_TEMPO_AQUISICAO;
          break;

        case 6: //"Quant Sensores"
          //Seta o item do sub-menu antes entrar no sub-menu.
          menu_set_value_indexes(&menu_cfg_quant_sensores, adcon_cfg_quant_sensores_atual);
          est_estado_novo = EST_ESTADO_MENU_CONF_QUANT_SENSORES;
          break;
          
        case 7: //"Enviar Dados"
          est_estado_novo = EST_ESTADO_ENVIAR_DADOS;
          break;
            
      }//switch (index)
    break; //BTN_START
    
    default:
      break;

  } //switch (botao)

}//est_estado_menu_principal()

/* Funcao que trata as transições do estado EST_ESTADO_MONITORA.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_monitora(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
  }
}//est_estado_monitora()

/**
 * Funcao que trata as transições do estado EST_MONITORA_GRAVA.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_monitora_grava(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
  }
}//est_estado_monitora()

/**
 * Funcao que trata as transições do estado EST_ESTADO_VER_AQUISICOES.
 * Controla a navegacao por todos os valores ja gravados na EEPROM.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_ver_aquisicoes(TBotao botao) {
  switch (botao) {
    case BTN_UP:
      if (est_ver_aquisicoes_index > 0) {
        est_ver_aquisicoes_index--;
        est_estado_novo = EST_ESTADO_VER_AQUISICOES;
      }
      break;
    case BTN_DOWN:
      if (serv_adcon_testa_indice_aquisicao_valida(est_ver_aquisicoes_index)) {
        est_ver_aquisicoes_index++;
        est_estado_novo = EST_ESTADO_VER_AQUISICOES;
      }
      break;
    case BTN_STOP:
      //est_troca_estado(EST_ESTADO_MENU_PRINCIPAL);
      est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
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
    est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
  }
}//est_estado_mostra_max_min()

/**
 * Funcao que trata as transições do estado EST_MENU_CONF_TEMPO_AQUISICAO.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_menu_conf_tempo_aquisicao(TBotao botao) {
  switch (botao) {
    case BTN_UP:
      if (menu_dec_index(&menu_cfg_tempo_aquisicao) != -1) {
        est_estado_novo = EST_ESTADO_MENU_CONF_TEMPO_AQUISICAO;
      }
      break;
    case BTN_DOWN:
      if ( menu_inc_index(&menu_cfg_tempo_aquisicao) != -1 ) {
        est_estado_novo = EST_ESTADO_MENU_CONF_TEMPO_AQUISICAO;
      }
      break;
    case BTN_STOP:
      menu_restore_index(&menu_cfg_tempo_aquisicao);
      est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
      break;
    case BTN_START:
      menu_confirma_index(&menu_cfg_tempo_aquisicao);
      serv_adcon_set_tempo_aquisicao_atual(menu_get_value_active(&menu_cfg_tempo_aquisicao));
      est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
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
    switch (botao) {
      case BTN_UP:
        if (menu_dec_index(&menu_cfg_quant_sensores) != -1) {
          est_estado_novo = EST_ESTADO_MENU_CONF_QUANT_SENSORES;
        }
        break;
      case BTN_DOWN:
        if ( menu_inc_index(&menu_cfg_quant_sensores) != -1 ) {
          est_estado_novo = EST_ESTADO_MENU_CONF_QUANT_SENSORES;
        }
        break;
      case BTN_STOP:
        menu_restore_index(&menu_cfg_quant_sensores);
        est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
        break;
      case BTN_START:
        menu_confirma_index(&menu_cfg_quant_sensores);
        serv_adcon_set_quant_sensores_atual((uint8_t)(menu_get_value_active(&menu_cfg_quant_sensores)));
        est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
        break;
      default:
        break;
    }//switch
}//est_estado_menu_conf_quant_sensores()

/**
 * Funcao que trata as transições do estado EST_MENU_LIMPAR.
  * @param botao o botão que foi pressionado.
 */
static void est_estado_limpar(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
  }
}//est_estado_limpar())

/**
 * Funcao que trata as transições do estado EST_ESTADO_ENVIAR_DADOS.
 * @param botao o botão que foi pressionado.
 */
static void est_estado_enviar_dados(TBotao botao) {
  if (botao == BTN_STOP) {
    est_estado_novo = EST_ESTADO_MENU_PRINCIPAL;
  }
}//est_estado_enviar_dados()

/**
 * Faz a troca para o novo estado e executa a ação entry desse novo estado.
 * Atualiza a variavel est_estado_atual = est_estado_novo.
 */
static void est_entra_estado_novo() {
 const char* texto;
  
  //Atualiza o estado atual.
  est_estado_atual = est_estado_novo;
  
  //Executa a ação "entry" do novo estado.

  switch (est_estado_atual) {

    case EST_ESTADO_INICIAL:

    //rs232_init() precisa ser antes do printf() e antes do lcd_puts() pois 
    //em modo __DEUBG essas funções utilizam a UART.      
    rs232_init();
    
    lcd_init();    

    //char teste[10];
    //sprintf(teste, "teste");
    //char teste2;
    //perror(&teste2);
    //fprintf(stderr, "teste\n");

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
      serv_adcon_inicializa_configuracoes_eeprom();
      serv_adcon_limpa_dados_eeprom();
    }
    else {
      //Chave de inicialização existe, então le as configurações da EEPROM.
      serv_adcon_le_configuracoes_eeprom();
    }
  
    //Inicializa os menus:
    menu_init(&menu_principal, menu_principal_itens, MENU_PRINCIPAL_TAM);
    menu_init(&menu_cfg_quant_sensores, menu_cfg_quant_sensores_itens, MENU_CFG_QUANT_SENSORES_TAM);
    menu_init(&menu_cfg_tempo_aquisicao, menu_cfg_tempo_aquisicao_itens, MENU_CFG_TEMPO_AQUISICAO_TAM);
  
    menu_set_value_indexes(&menu_cfg_quant_sensores, adcon_cfg_quant_sensores_atual);
    menu_set_value_indexes(&menu_cfg_tempo_aquisicao,  adcon_cfg_tempo_aquisicao_atual);
  
    lcd_goto(2, 0);
    lcd_puts("Pronto!         ");

    est_equipamento_inicializado = 1;
    
    break;

    case EST_ESTADO_MENU_PRINCIPAL:
      lcd_clear();
      texto = menu_get_text_nav(&menu_principal);
      lcd_puts(texto);
      break; //EST_MENU_PRINCIPAL
      
    case EST_ESTADO_MONITORA:
      //serv_adcon_aquisicao_print();
      serv_adcon_aquisicao();
      serv_adcon_print();
      break; //EST_CAPTURA_E_MOSTRA
      
    case EST_ESTADO_MONITORA_GRAVA:
      //Marca que o monitramento será iniciado.
      serv_adcon_monitora_grava = 1;
      //Faz uma amostragem.
      serv_adcon_aquisicao_print_grava();
      break; //EST_CAPTURA_E_GRAVA
      
    case EST_ESTADO_VER_AQUISICOES:
      serv_adcon_print_aquisicao_da_eeprom(est_ver_aquisicoes_index);
      break; //EST_MOSTRA_TODOS
    
    case EST_ESTADO_MOSTRA_MAX_MIN:
      serv_adcon_print_max_min();
      break; //EST_MOSTRA_MAX_MIN
      
    case EST_ESTADO_LIMPAR:
      //Recém entrou no estado.
      lcd_clear();
      lcd_puts("Limpando...");
      //Inicializa toda a EEPROM com os valores default.
      serv_adcon_inicializa_configuracoes_eeprom();
      serv_adcon_limpa_dados_eeprom();
      lcd_clear();
      lcd_puts("Tudo Limpo.");
      break; //EST_LIMPAR
      
    case EST_ESTADO_MENU_CONF_TEMPO_AQUISICAO:
      lcd_clear();
      texto = menu_get_text_nav(&menu_cfg_tempo_aquisicao);
      lcd_puts(texto); 
      break; //EST_MENU_CONF_TEMPO_AQUISICAO

    case EST_ESTADO_MENU_CONF_QUANT_SENSORES:
      lcd_clear();
      texto = menu_get_text_nav(&menu_cfg_quant_sensores);
      lcd_puts(texto);
      break; //EST_MENU_CONF_QUANT_SENSORES
      
    case EST_ESTADO_ENVIAR_DADOS:
      serv_adcon_envia_rs232_amostras_gravadas_eeprom();
      break; //EST_ENVIAR_DADOS

    default:
      break;

    }//switch (novo_estado)
     
}//est_entra_estado_novo()

//===== Final do Arquivo =====================================================
