//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "driverlib.h"
#include <adc14.h>

const int max_period = 1000;
int cl0 = 0, cl1 = 0;
int limit = 1000;
int leftButtonPressed = 0;
int rightButtonPressed = 0;
uint8_t ascending_number = 0X01;
char dataString[6];
volatile uint8_t dataIndex = 0;
volatile uint8_t messageReceived = 0;

uint8_t property[4];
int receivedInteger;


int includesWhiteSpaces(char string[6]){
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

void main(void)
{

    WDT_A_hold(WDT_A_BASE);  // Stop watchdog timer

    init_io();
    init_PWM_pins();

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
    UART_enableModule(EUSCI_A0_BASE);

    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
    Interrupt_enableMaster();
    
    while(1){
        if(messageReceived == 1){
             process_controls(dataString);
             messageReceived = 0;
             dataString[0] = '\0';
         }
    }
}

void UARTSendChar(char c) {
    while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    MAP_UART_transmitData(EUSCI_A0_BASE, (uint8_t)c);
}

void UARTSendString(const char *str) {
    while (*str) {
        UARTSendChar(*str++);
    }
}
