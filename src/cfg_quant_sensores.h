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
    
  //Quantidade máxima possível de sensores.
  #define CFG_QUANT_SENSORES_MAX 4
  
  //É a quantidade de sensores atualmente configurada.
  extern uint8_t cfg_quant_sensores_atual;
  
  //É a quantidade de sensores que estava configurada a última vez que
  //foi feito um monitoramento.
  //extern uint8_t cfg_quant_sensores_amostrados;
  
#ifdef	__cplusplus
}
#endif

#endif	/* CFG_QUANT_SENSORES_H */
