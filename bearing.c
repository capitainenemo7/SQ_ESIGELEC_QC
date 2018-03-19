/*
 * bearing.c
 *
 *  Created on: 19 mars 2018
 *      Author: Antoine
 */

#include "bearing.h"
#include "compass.h"
#include <msp430g2553.h>

uint8_t new_brearing(uint16_t actual_bearing, int16_t relative_angle, int16_t* new_bearing)
{
    if(new_bearing == 0)
    {
        return 1;           /* error : nowhere to write the new data */
    }
    else
    {
        *new_bearing = ((actual_bearing + 3600) + (relative_angle % 3600)) % 3600;
        return 0;
    }
}