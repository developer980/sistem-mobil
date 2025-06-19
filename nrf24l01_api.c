/*
 * nrf24l01_api.c
 *
 *  Created on: Apr 21, 2025
 *      Author: tudor
 */
#include "nrf24l01.h"
#include <stdint.h>
#include "driverlib.h"
#include "nrf24l01_api.h"

#define W_REGISTER     0x20
#define W_TX_PAYLOAD   0xA0
#define CONFIG         0x00
#define TX_ADDR        0x10
#define RX_ADDR_P0     0x0A
#define STATUS         0x07

void nrf24_init(void)
{
    nrf24_CE(0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN1); // CSN HIGH
    delay_function(5); // wait for power on

    nrf24_write_register(0x00, 0x08); // CONFIG → default (mask interrupts)
    nrf24_write_register(0x01, 0x00); // Disable Auto Ack
    nrf24_write_register(0x02, 0x01); // Enable Pipe 0
    nrf24_write_register(0x03, 0x03); // Address width = 5
    nrf24_write_register(0x05, 0x02); // RF_CH = 2
    nrf24_write_register(0x06, 0x07); // RF_SETUP: 1Mbps, 0dBm
}

void nrf24_config(uint8_t channel, uint8_t payload_length)
{
    uint8_t address[5] = {'0', '0', '0', '0', '1'};  // Same as "00001" in Arduino

    // Set RX address on pipe 0 (for auto-acknowledgment, even if you're not using it)
    nrf24_write(0x0A, address, 5, CLOSE);  // RX_ADDR_P0

    // Set TX address
    nrf24_write(0x10, address, 5, CLOSE);  // TX_ADDR

    nrf24_write_register(0x05, channel);       // RF_CH
    nrf24_write_register(0x11, payload_length);   // RX_PW_P0
}

void nrf24_powerUpRx(void)
{
    nrf24_write_register(0x00, 0x0F);  // Power up, PRIM_RX
    delay_function(2);
    nrf24_CE(1); // Set CE HIGH to start listening
}

void NRF24_powerUpTx(void) {
    nrf24_CE(0);  // CE low while configuring
   nrf24_write_register(CONFIG, (1 << PWR_UP) | (0 << PRIM_RX)); // Power up, TX mode
   delay_function(2); // Wait for power-up (1.5ms minimum)
   nrf24_CE(1); // Enable CE pin to start transmitting
   delay_function(2); // Ensure the CE pin is high long enough for transmission
}
