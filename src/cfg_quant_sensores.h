/* 
 * File:   sensores.h
 * Author: 
 * 
 */

#ifndef CFG_QUANT_SENSORES_H
#define	CFG_QUANT_SENSORES_H

#ifdef	__cplusplus
extern "C" {
#endif

  #include <stdint.h>
    
  //Quantidade m�xima poss�vel de sensores.
  #define CFG_QUANT_SENSORES_MAX 4
  
  //� a quantidade de sensores atualmente configurada.
  extern uint8_t cfg_quant_sensores_atual;
  
  //� a quantidade de sensores que estava configurada a �ltima vez que
  //foi feito um monitoramento.
  //extern uint8_t cfg_quant_sensores_amostrados;
  
#ifdef	__cplusplus
}
#endif

#endif	/* CFG_QUANT_SENSORES_H */
