/*
 * NRF24L01.h
 *
 *  Created on: Jan 16, 2023
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_RF_NRF24L01_H_
#define LIBRARIES_RF_NRF24L01_H_

void Spi_NRF24L01_Setup();
void NRF24L01_Setup();
void NRF24L01_Mode_Tx(uint8_t *address, uint8_t channel);
uint8_t NRF24L01_Transmit(uint8_t *data);

void NRF24L01_Receive(uint8_t *data);
uint8_t NRF24L01_isDataAvailable(int pipenum);
void NRF24L01_Mode_Rx(uint8_t *Address, uint8_t channel);

void RF_SendData(uint8_t *address, uint8_t channel, uint8_t nrOfFunction, uint8_t* data, uint8_t length);

extern uint8_t rxAddress[];
extern uint8_t txAddress[];
extern uint8_t rxBuff[32];
extern uint8_t txBuff[32];

#define NRF_CHANNEL_RX 10
#define NRF_CHANNEL_TX 10

#define NRF_FUNCTION_RGB 1


//
// memory map
//
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

// Instruction Mnemonics
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

#endif /* LIBRARIES_RF_NRF24L01_H_ */
