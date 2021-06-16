/* 
 * File:   LED.h
 * Author: adam
 *
 * Created on 7 June 2021, 10:50 PM
 */

#ifndef LED_H
#define	LED_H

#include "board.h"

#define ledInit() {LED0_DDR=OUTPUT; LED0=0;}
#define ledOn()   LED0=1
#define ledOff()  LED0=0

#endif	/* LED_H */

