#include <msp430g2553.h>
#include "system.h"
#include <stdint.h>
#include "intrinsics.h"
#include "timeout.h"
#include "display.h"
#include "compass.h"
#include "control_motors.h"
#include "scheduler.h"
#include "bearing.h"
#include "control_encoders.h"

#include "non_blocking.h"
#include "blocking_movements.h"
#include "Turn_path.h"

#define SPEED 50
#define CENT_MS 1200000
/*

 * main.c

 */

void main(void) {
    distance_type dist;
    uint8_t in_movement = 0,direction = 0;
    uint16_t actual_bearing =0, target_bearing = 0;

    //WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer
    init_timeout();
    Clock_graceInit_DCO_12M();
    init_display();
    Init_encoders_distance();
    Init_motors();
    compass_init();


    __enable_interrupt();

    while(1)
    {
        if(in_movement == 0)
        {
            while(Read_compass_16(&actual_bearing) != 0)
            {
                show_string("Err0");
            }


            switch(direction)
            {
            case 0:
                /*Compute target bearing */
                new_bearing(actual_bearing,900,&target_bearing);
                nb_turn_step_plus_bearing(FORWARD,RIGHT,SPEED*0.5,SPEED,target_bearing);
                break;
            case 1:
                /*Compute target bearing */
                new_bearing(actual_bearing,900,&target_bearing);
                nb_turn_step_plus_bearing(BACKWARD,LEFT,SPEED*0.5,SPEED,target_bearing);
                break;

            case 2:
                stop_nb(1000);
                break;
            default:;
            }                             /* Change direction */
            in_movement = 1;                                        /* Now we are in movement */
        }
        /*if the robot is moving check if it must stop*/
        else
        {
            /*Every 2ms we enter in the condition */
            if(check_and_clear_Tick_out() == 1)
            {
                show_number(direction);
                switch(direction)
                {
                    case 0:
                        if((check_stop_bearing() == 0))
                        {/* If the robot need to stop */
                            in_movement = 0;
                            direction = 1;/* we are ready to launch new movement */
                        }
                        else
                            in_movement = 1;
                        break;
                    case 1:
                        if((check_stop_bearing() == 0))
                        {/* If the robot need to stop */
                            in_movement = 0;
                            direction = 2;/* we are ready to launch new movement */
                        }
                        else
                            in_movement = 1;
                        break;
                    case 2:
                        if((check_stop() == 0))
                        {/* If the robot need to stop */
                            in_movement = 0;
                            direction = 0;
                        }/* we are ready to launch new movement */
                        else
                            in_movement = 1;
                        break;
                }

            }
        }

        /* Do something to show that we are using not blocking functions */
        //Read_distance(LEFT, &dist);                                 /* We read the distance on the left wheel */
        //show_number(dist.turns);                                    /* We print the number of turn we made on the display */
    }
}
