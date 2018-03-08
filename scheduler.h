/*
 * scheduler.h
 *
 *  Created on: 29/1/2018
 *      Author: amarti
 */

#include <msp430g2553.h>
#include "stdint.h"

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

void callback_2ms(void);
uint8_t check_and_clear_Tick_out(void);

#endif /* SCHEDULER_H_ */
