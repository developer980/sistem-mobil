/*
 * gpio.c
 *
 *  Created on: Apr 19, 2025
 *      Author: tudor
 */

#include "msp.h"
#include "driverlib.h"
#include <adc14.h>

Timer_A_PWMConfig pwmConfigLeft = {
    TIMER_A_CLOCKSOURCE_SMCLK, // set timer source as SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    6000,
    TIMER_A_CAPTURECOMPARE_REGISTER_3,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0
};

Timer_A_PWMConfig pwmConfigRight = {
    TIMER_A_CLOCKSOURCE_SMCLK, // set timer source as SMCLK
    TIMER_A_CLOCKSOURCE_DIVIDER_1,
    6000,
    TIMER_A_CAPTURECOMPARE_REGISTER_4,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0
};


//Timer_A_PWMConfig pwmConfig = {
//    TIMER_A_CLOCKSOURCE_SMCLK, // set timer source as SMCLK
//    TIMER_A_CLOCKSOURCE_DIVIDER_64,
//    10000,
//    TIMER_A_CAPTURECOMPARE_REGISTER_1
//    TIMER_A_OUTPUTMODE_RESET_SET,
//    3300
//};

//uint_fast16_t clockSource;
//uint_fast16_t clockSourceDivider;
//   uint_fast16_t timerPeriod;
//   uint_fast16_t compareRegister;
//   uint_fast16_t compareOutputMode;
//   uint_fast16_t dutyCycle;

void init_io(){

//    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;          // Stop watchdog timer

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
}


void init_PWM_pins(){
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigLeft);
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfigRight);
}


void init_analog_pins(){

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4 | GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    ADC14_enableModule();

    // Initialize ADC14:
    // Parameters:
    //  ADC_CLOCKSOURCE_SMCLK: Use SMCLK as the ADC clock source
    //  ADC_PREDIVIDER_1: No pre-divider
    //  ADC_DIVIDER_4: Divide the clock by 4 (e.g., 12MHz SMCLK / 4 = 3MHz ADC clock)
    //  ADC_NORUNMODE_SEQUENCE_OF_CHANNELS: Use sequence of channels mode for multiple conversions
    //  ADC_MANUAL_ITERATION: Manually start each sequence
    //  ADC_ONESHOT_ADC_MODE: Perform one sequence conversion per trigger
    ADC14_initModule(
            ADC_CLOCKSOURCE_SMCLK,       // MCLK as ADC clock source
        ADC_PREDIVIDER_1,           // Predivide by 1
        ADC_DIVIDER_1,              // Divide by 1
        0                           // No internal channels (like temp sensor, etc.)
    );
    // Configure the ADC Memory Channel (ADC_MEM0 for A0 in this example)
    // Parameters:
    //  ADC_MEM0: The memory register to store the conversion result
    //  ADC_INPUT_A0: The analog input channel (P5.5 maps to A0 on many LaunchPads)
    //  ADC_VREF_AVCC_AVSS: Use AVCC (3.3V) as the positive reference and AVSS (GND) as the negative reference
    //  ADC_ENDOFSEQUENCE: This flag is important if you're chaining multiple ADC conversions.
    //                   Set it to indicate the last channel in a sequence.
    ADC14_configureSingleSampleMode(ADC_MEM0, ADC_INPUT_A0);

    // --- Interrupt-specific configuration ---
    // Enable interrupt for ADC_MEM0 (ADC_INT0)
    ADC14_enableInterrupt(ADC_INT0);

    // Clear any pending interrupt flags (good practice before enabling)
    ADC14_clearInterruptFlag(ADC_INT0);

    // Enable ADC conversion (ADC is ready to convert when triggered)
    ADC14_enableConversion();

    Interrupt_setPriority(ADC14_IRQn, 0); // Set priority (0 is highest)
    Interrupt_enableInterrupt(ADC14_IRQn); // Enable in NVIC

    // Globally enable interrupts
    Interrupt_enableMaster();

}
