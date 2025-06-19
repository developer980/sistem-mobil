/*
 * nrf24l01_api.h
 *
 *  Created on: Apr 21, 2025
 *      Author: tudor
 */

#ifndef NRF24L01_API_H_
#define NRF24L01_API_H_



#include <stdint.h>

void nrf24_init(void);
void nrf24_config(uint8_t channel, uint8_t payload_length);
void nrf24_powerUpRx(void);

#endif /* NRF24L01_API_H_ */
