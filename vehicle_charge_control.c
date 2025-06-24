/*
 * vehicle_charge_control.c
 *
 *  Created on: Jun 24, 2025
 *      Author: tudor
 */

#include "msp.h"
#include "driverlib.h"

void start_vehicle_charging(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN4);
}

void stop_vehicle_charging(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN4);
}
