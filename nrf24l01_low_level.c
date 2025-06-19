#include "nrf24l01.h"
#include "driverlib.h"


const eUSCI_SPI_MasterConfig spiConfig = {
    EUSCI_B_SPI_CLOCKSOURCE_SMCLK,      // Use SMCLK as the clock source
    3000000,                           // SMCLK = 12 MHz (adjust as per your system clock)
    1000000,                             // SPI clock = 500 kHz (adjust for NRF24L01 max 10 MHz)
    EUSCI_B_SPI_MSB_FIRST,              // Most significant bit first
    EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,  // SPI mode 0 (CPOL = 0, CPHA = 0)
    EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
    EUSCI_B_SPI_3PIN                    // 3-pin SPI (MOSI, MISO, SCLK)
};

/*start of low level functions, specific to the mcu and compiler*/

/*delay in miliseconds*/
void delay_function(uint32_t duration_ms)
{
    for(uint32_t i = 0; i < (duration_ms * 1000); i++)
    {
        __delay_cycles(12);  // 12 cycles = 1us at 12 MHz
    }
}

/*contains all SPI configuations, such as pins and control registers*/
/*SPI control: master, interrupts disabled, clock polarity low when idle, clock phase falling edge, clock up tp 1 MHz*/
void SPI_Initializer()
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);

    SPI_initMaster(EUSCI_B0_BASE, &spiConfig);
    SPI_enableModule(EUSCI_B0_BASE);
}

/*contains all CSN and CE pins gpio configurations, including setting them as gpio outputs and turning SPI off and CE '1'*/
void pinout_Initializer()
{
    //GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1);
    nrf24_CE(1);
}

/*CSN pin manipulation to high or low (SPI on or off)*/
void nrf24_SPI(uint8_t input)
{
    if(input == 1) {
        // Enable SPI comm (CSN low)
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1);
    } else {
        // Disable SPI comm (CSN high)
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN1);
    }
}

/*1 byte SPI shift register send and receive routine*/
uint8_t SPI_send_command(uint8_t command)
{
    while (!(SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

    SPI_transmitData(EUSCI_B0_BASE, command);

    // Wait until RX buffer has received something
    while (!(SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT)));

    return SPI_receiveData(EUSCI_B0_BASE);
}

/*CE pin maniplation to high or low*/
void nrf24_CE(uint8_t input)
{
    if(input == 1){
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
    }

    else{
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
    }
}

void nrf24_write_register(uint8_t reg, uint8_t value)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN1); // CSN LOW

    SPI_transmitData(EUSCI_B0_BASE, 0x20 | reg); // Write command
    while (!SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_SPI_TRANSMIT_INTERRUPT));
    while (!SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_SPI_RECEIVE_INTERRUPT));
    SPI_receiveData(EUSCI_B0_BASE); // Dummy read

    SPI_transmitData(EUSCI_B0_BASE, value);       // Value
    while (!SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_SPI_TRANSMIT_INTERRUPT));
    while (!SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_SPI_RECEIVE_INTERRUPT));
    SPI_receiveData(EUSCI_B0_BASE); // Dummy read

    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN1); // CSN HIGH
}
