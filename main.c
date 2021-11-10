#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "serial.h"
#include "timer.h"


void main (void) {

	i2c_init();
	uart_init();

	sei();

	
    uint8_t Eeprom_data;

		printf_P(PSTR("Manufature CODE: "));
		Eeprom_data = eeprom_read_byte(0xFA);
		printf_P(PSTR("%x\n"),Eeprom_data);

		printf_P(PSTR("Device CODE: "));
		Eeprom_data = eeprom_read_byte(0xFB);
		printf_P(PSTR("%x\n"),Eeprom_data);

		_delay_ms(100);

		printf_P(PSTR("32-bit Serial Number\n: "));
		Eeprom_data = eeprom_read_byte(0xFC);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		Eeprom_data = eeprom_read_byte(0xFD);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		Eeprom_data = eeprom_read_byte(0xFE);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		Eeprom_data = eeprom_read_byte(0xFF);
		printf_P(PSTR("%x\n"),Eeprom_data);
	

   

	while (1) {
		
        

	}
}

