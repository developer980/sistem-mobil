//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "driverlib.h"
#include <adc14.h>
//#include "nrf24l01.h"

const int max_period = 1000;
int cl0 = 0, cl1 = 0;
int limit = 1000;
int leftButtonPressed = 0;
int rightButtonPressed = 0;
uint8_t ascending_number = 0X01;
char dataString[6];
volatile uint8_t dataIndex = 0;
volatile uint8_t messageReceived = 0;
//volatile uint8_t adc_conversion_complete = 0;
//volatile uint8_t adc_conversion_result = 0;
uint8_t property[4];
int receivedInteger;


int includesWhiteSpaces(char string[6]){
//    printf("analysing %s", string);
    if(string[0] == '\0'){
        return 1;
    }

    for(int i = 0; string[i] != '\0'; i++){
        if(string[i] == '\n' || string[i] == ' '){
            return 1;
        }
    }

    return 0;
}


void EUSCIA0_IRQHandler(void){
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        char dataSection = UART_receiveData(EUSCI_A0_BASE);

        if (dataIndex < 5) {
            if(dataSection != '\n'){
                dataString[dataIndex++] = dataSection;
            }
        } else if(dataIndex >= 5 || dataSection == '\n') {
            dataString[dataIndex] = '\0'; // Null terminate
            if(!includesWhiteSpaces(dataString)){
                messageReceived = 1;
            }

//            printf("DataString: %s \n", dataString);
            dataIndex = 0;
        }
    }
}


//void ADC14_IRQHandler(void) {
//    uint64_t status;
//
//    // Get the interrupt status to determine which flag caused the interrupt
//    status = ADC14_getEnabledInterruptStatus();
//    ADC14_clearInterruptFlag(status); // Clear the triggered interrupt flag(s)
//
//    // Check if the ADC_INT0 (for ADC_MEM0) interrupt occurred
//    if (status & ADC_INT0) {
//        adc_conversion_result = ADC14_getResult(ADC_MEM0); // Read the conversion result
//        adc_conversion_complete = 1; // Set flag to notify main loop
//    }
//
//    // If you had other ADC interrupts enabled (e.g., ADC_INT1 for ADC_MEM1), you'd handle them here too
//}



//void TA0_0_IRQHandler(void)
//{
//    // Clear the interrupt flag
//    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
//}

void main(void)
{

    WDT_A_hold(WDT_A_BASE);  // Stop watchdog timer



     //Configure Timer_A0
//    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
//
//
//    // Configure PORT1_IRQ
////    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
//    NVIC_EnableIRQ(PORT1_IRQn);
//    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
//    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
//    TIMER_A0->CCR[0] = max_period; // period value
//    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP;
//    __enable_irq();  // Enable global interrupts


    // configure  PWM

//    pwm(500);
//    pwm1(0);


    init_io();
    init_PWM_pins();
//    init_analog_pins();

    // Set DCO frequency to 12MHz
    CS_setDCOFrequency(12000000);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Configure UART pins (P1.2 = RX, P1.3 = TX)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);


    // UART Configuration
    eUSCI_UART_Config uartConfig = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,     // SMCLK Clock Source
        78,                                   //(9600 baud @12MHz)
        2,                                   // UCxBRF = 2
        0x20,                                // UCxBRS = 0x20
        EUSCI_A_UART_NO_PARITY,              // No Parity
        EUSCI_A_UART_LSB_FIRST,              // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,           // One stop bit
        EUSCI_A_UART_MODE,                   // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Oversampling
    };



    // Initialize and enable UART
    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_enableModule(EUSCI_A0_BASE);//    __delay_cycles(50000);

    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
    Interrupt_enableMaster();

    // Transmit data




    while(1){

//        MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
//        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN2);
//        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN3);
//        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
//        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3);
        if(messageReceived == 1){
//             printf("Received message: %s \n", dataString);
             process_controls(dataString);
             messageReceived = 0;
             dataString[0] = '\0';
         }

//        if(adc_conversion_complete == 1){
//            printf("analog value %d", adc_conversion_result);
//        }
    }
}



//Timer_A_PWMConfig;{
//    uint_fast16_t clockSource;
//    uint_fast16_t clockSourceDivider;
//    uint_fast16_t timerPeriod;
//    uint_fast16_t compareRegister;
//    uint_fast16_t compareOutputMode;
//    uint_fast16_t dutyCycle;
//}


//void pwm(int value){
//    P2->DIR |= BIT7;      // P2.7 set as output
//    P2->SEL0 |= BIT7;
//    P2->SEL1 &= ~BIT7;
//    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set
//    TIMER_A0->CCR[1] = value;
//}
//
//void pwm1(int value){
//    P2->DIR |= BIT6;      // P2.7 set as output
//    P2->SEL0 |= BIT6;
//    P2->SEL1 &= ~BIT6;
//    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // CCR2 reset/set
//    TIMER_A0->CCR[2] = value;
//}
void UARTSendChar(char c) {
    while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    MAP_UART_transmitData(EUSCI_A0_BASE, (uint8_t)c);
}

void UARTSendString(const char *str) {
    while (*str) {
        UARTSendChar(*str++);
    }
}
