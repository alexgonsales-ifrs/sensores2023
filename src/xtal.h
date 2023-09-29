/* 
 * File:   xtal.h
 * Author: ti
 *
 * Created on 25 de Dezembro de 2013, 20:18
 */

#ifndef XTAL_H
#define	XTAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#define _XTAL_FREQ 4000000

//Se estiver em modo __DEBUG então redefine as macros __delay_ms(x) e __delay_us(x)
//senão o simulador vai ficar lento (ou travar).
#ifdef __DEBUG
  //#undef __delay_ms(x)
  //#define __delay_ms(x) _delay(1)
  //#undef __delay_us(x)
  //#define __delay_us(x) _delay(1)
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* XTAL_H */
