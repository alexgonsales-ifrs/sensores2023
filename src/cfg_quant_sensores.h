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
    
  #define CFG_QUANT_SENSORES_MAX 4
    
  extern uint8_t cfg_quant_sensores_atual;
  extern uint8_t cfg_quant_sensores_amostrados;
  
#ifdef	__cplusplus
}
#endif

#endif	/* CFG_QUANT_SENSORES_H */
