#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

#include "i2c.h"

// ATmega328p megaAVR Datasheet page 239,22.9 Register Description

void i2c_init(void)
{ //initiating I2C setting bit rate to 100Khz

	/* TWSR – TWI Status Register
	   TWPS1  TWPS0   Prescaler Value
       0      0         1
    */
	TWSR |= (0 << TWPS1); // setting presclar value to 4
	TWSR |= (1 << TWPS0); // setting bit rate
	//TWSR = 0x00;
	TWBR = 0x12; //set SCL to 100KHz; from calculating;  SCL frequency = CPU Clock frequency/16+ 2*(TWBR) (prescalar) =72( decimal value)

	TWCR = (1 << TWEN); // enabling TWI (two wire interface)
}

void i2c_meaningful_status(uint8_t status)
{
	switch (status)
	{
	case 0x08: // START transmitted, proceed to load SLA+W/R
		printf_P(PSTR("START\n"));
		break;
	case 0x10: // repeated START transmitted, proceed to load SLA+W/R
		printf_P(PSTR("RESTART\n"));
		break;
	case 0x38: // NAK or DATA ARBITRATION LOST
		printf_P(PSTR("NOARB/NAK\n"));
		break;
	// MASTER TRANSMIT
	case 0x18: // SLA+W transmitted, ACK received
		printf_P(PSTR("MT SLA+W, ACK\n"));
		break;
	case 0x20: // SLA+W transmitted, NAK received
		printf_P(PSTR("MT SLA+W, NAK\n"));
		break;
	case 0x28: // DATA transmitted, ACK received
		printf_P(PSTR("MT DATA+W, ACK\n"));
		break;
	case 0x30: // DATA transmitted, NAK received
		printf_P(PSTR("MT DATA+W, NAK\n"));
		break;
	// MASTER RECEIVE
	case 0x40: // SLA+R transmitted, ACK received
		printf_P(PSTR("MR SLA+R, ACK\n"));
		break;
	case 0x48: // SLA+R transmitted, NAK received
		printf_P(PSTR("MR SLA+R, NAK\n"));
		break;
	case 0x50: // DATA received, ACK sent
		printf_P(PSTR("MR DATA+R, ACK\n"));
		break;
	case 0x58: // DATA received, NAK sent
		printf_P(PSTR("MR DATA+R, NAK\n"));
		break;
	default:
		printf_P(PSTR("N/A %02X\n"), status);
		break;
	}
}

//Generate start condition
/*We need to set TWSTA and stop TWSTO bits along with TWINT and TWEN bits for a start.
 After the start signal is sent, we need to wait for status (until TWINT resets zero).*/
inline void i2c_start()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //Two wire control register TWCR
	while (!(TWCR & (1 << TWINT)))
		;
}

//Generate stop condition
inline void i2c_stop()
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while (TWCR & (1 << TWSTO))
		;
	//TWCR = (1 << TWINT) | (0 << TWSTO) | (1 << TWEN) | (1 << TWINT); //bus realeased
	//while (!(TWCR & (1 << TWINT))){}
}

//Return status of ongoing operation
inline uint8_t i2c_get_status(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8; //Two wire status register
	return status;
}

//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
inline void i2c_xmit_addr(uint8_t address, uint8_t rw)
{
	TWDR = (address & 0xfe) | (rw & 0x1); //load SLA_W into register TWDR
	TWCR = (1 << TWINT) | (1 << TWEN);	  // Clear TWINT bit in TWCR to start transmission of address
	while ((TWCR & (1 << TWINT)) == 0)
		; //kames sure that PKt has been sent
}

//Send/write just one byte
void i2c_xmit_byte(uint8_t data)
{
	TWDR = data; //Two Wire Data Register TWDR
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0)
		; //This indicates that the DATA has been transmitted, and ACK/NACK has been received.
}

//Read just one byte with ACK //for more bytes
uint8_t i2c_read_ACK()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); //TWEA enable ack
	while ((TWCR & (1 << TWINT)) == 0)
		;
	return TWDR;
}

//Read just one byte without ACK //for the last byte
uint8_t i2c_read_NAK()
{
	TWCR = (1 << TWINT) | (1 << TWEN); //ack bit not set
	while ((TWCR & (1 << TWINT)) == 0)
		;
	return TWDR;
}

inline void eeprom_wait_until_write_complete()
{
	uint8_t status = 0;
	while (status != 0x18)
	{ //check if slave is sending ACK, if no ACK writing is in progress so wait
		i2c_start();
		i2c_xmit_addr(EEPROM_addr, 0);
		status = i2c_get_status();
	}
}

uint8_t eeprom_read_byte(uint8_t addr)
{
	//Generate start condition
	i2c_start();

	//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
	i2c_xmit_addr(EEPROM_addr, 0);

	//send address to wirte at
	i2c_xmit_byte(addr);

	//Generate start condition again
	i2c_start();

	//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
	i2c_xmit_addr(EEPROM_addr, 1);

	//Read just one byte without ACK //for the last byte
	uint8_t received = i2c_read_NAK();

	//Generate stop condition
	i2c_stop();
	return received;
}

void eeprom_write_byte(uint8_t addr, uint8_t data)
{
	//Generate start condition
	i2c_start();
	//i2c_meaningful_status(i2c_get_status());

	//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
	i2c_xmit_addr(EEPROM_addr, 0);
	//i2c_meaningful_status(i2c_get_status());

	//send the address to be written to
	i2c_xmit_byte(addr);
	//i2c_meaningful_status(i2c_get_status());

	//Send/write just one byte
	i2c_xmit_byte(data);

	//i2c_meaningful_status(i2c_get_status());

	//Generate stop condition
	i2c_stop();

	//wait until write has finished
	eeprom_wait_until_write_complete();
	//i2c_meaningful_status(i2c_get_status());
}

void eeprom_write_page(uint8_t addr, uint8_t *data)
{
	uint8_t chardata = *data;

	//Generate start condition
	i2c_start();
	//i2c_meaningful_status(i2c_get_status());

	//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
	i2c_xmit_addr(EEPROM_addr, 0);
	//i2c_meaningful_status(i2c_get_status());

	//send the address to be written to
	i2c_xmit_byte(addr);
	//i2c_meaningful_status(i2c_get_status());

	for (int j = 0; j < Pagesize; j++) //length of string,pagesize = 8
	{
		//Send/write just one byte
		i2c_xmit_byte(chardata); //Upon receipt of each word, the 3 lower-order Address Pointer bits are internally incremented by ‘1’.
		//The higher-order five bits of the word address remain constant.

		chardata = *(++data); //PreIncrementing the pointer to next char

		//eeprom_wait_until_write_complete(); //wait until write has finished
		//i2c_meaningful_status(i2c_get_status());
	}

	i2c_stop();
	eeprom_wait_until_write_complete(); //wait until write has finished
}

void eeprom_sequential_read(uint8_t *buff, uint8_t Initial_addr, uint8_t len)
{
	//uint8_t eeprom_sequential_read(uint8_t start_addr)

	i2c_start();
	//i2c_meaningful_status(i2c_get_status());

	//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
	i2c_xmit_addr(EEPROM_addr, 0);
	// i2c_meaningful_status(i2c_get_status());

	//send address to read from
	i2c_xmit_byte(Initial_addr);
	// i2c_meaningful_status(i2c_get_status());

	//Generate start condition again
	i2c_start();
	// i2c_meaningful_status(i2c_get_status());

	//Send SLA+ R/W (Address Pkt consisting of slave address and READ and WRITE bit)
	i2c_xmit_addr(EEPROM_addr, 1);
	// i2c_meaningful_status(i2c_get_status());

	for (int i = 0; i < len; i++) //loop through the entire array
	{
		uint8_t chardata2 = i2c_read_ACK();
		//i2c_meaningful_status(i2c_get_status());
		//uart_putchar(chardata2, 0);

		if (i == len - 1)
		{
			chardata2 = i2c_read_NAK();
			//uart_putchar(chardata2, 0);
			buff[i] = chardata2;
		}
		else
		{
			buff[i] = chardata2;
		}
	}

	i2c_stop();
}
