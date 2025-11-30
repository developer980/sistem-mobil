/*
 * gpio.c
 *
 *  Created on: Apr 19, 2025
 *      Author: tudor
 */

#include "msp.h"
#include "driverlib.h"
#include <adc14.h>

// Configure timer A registers 3 and 4 for PWM signals for motor controls

Timer_A_PWMConfig pwmConfigLeft = {
    TIMER_A_CLOCKSOURCE_SMCLK, // set timer source as SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    4000,
    TIMER_A_CAPTURECOMPARE_REGISTER_3,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0
};

Timer_A_PWMConfig pwmConfigRight = {
    TIMER_A_CLOCKSOURCE_SMCLK, // set timer source as SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    4000,
    TIMER_A_CAPTURECOMPARE_REGISTER_4,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0
};

// Initiate digiral pins

void init_io(){
   // GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    // Set P1.1 and P1.4 as input (leave direction as input)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);

    // Set P1.1 and P1.4 as input with pull-up resistors
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);

    // Set interrupt to trigger on high-to-low transition (falling edge)
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);

    // Clear interrupt flags for P1.1 and P1.4
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);

    // Enable interrupts for P1.1 and P1.4
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4);

    // Set P1.0 (LED1) as output and turn it off
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    // Set P3.2 (LED1) as output and turn it off
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);

    // Set P3.3 (LED1) as output and turn it off
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN3);

    // Set P4.1 (LED1) as output and turn it off
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);

    // Set P4.3 (LED1) as output and turn it off
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN4);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN5);
}

// Initiate PWM pins
void init_PWM_pins(){

    // Declare P2.6 and P2.7 for PWM transmitting
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    // Generate PWM signal at the declared pins
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigLeft);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigRight);
}


