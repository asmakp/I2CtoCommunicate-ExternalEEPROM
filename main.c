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
        
		// DELUPPGIFT 1: KONFIGURERA OCH ANVÄND I 2C GENERELLT
		/*printf_P(PSTR("Manufature CODE: "));
		Eeprom_data = eeprom_read_byte(0xFA);
		printf_P(PSTR("%x\n"),Eeprom_data);

		printf_P(PSTR("Device CODE: "));
		Eeprom_data = eeprom_read_byte(0xFB);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		printf_P(PSTR("32-bit Serial Number\n: "));
		Eeprom_data = eeprom_read_byte(0xFC);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		Eeprom_data = eeprom_read_byte(0xFD);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		Eeprom_data = eeprom_read_byte(0xFE);
		printf_P(PSTR("%x\n"),Eeprom_data);
		
		Eeprom_data = eeprom_read_byte(0xFF);
		printf_P(PSTR("%x\n"),Eeprom_data);*/
	
     
    uint8_t Read_data;

	char name[] = "Asma";

	uint8_t Initial_addr = 0x10;
   

	while (1) {

	    for(int i = 0; i < sizeof(name); i++)       //to loop until all the characters in the array are written
	   {
      	  eeprom_write_byte(Initial_addr,name[i]);  //send each char
			Initial_addr++;                         //increment address to acomodate each char
	   }
      // _delay_ms(100);
       Initial_addr = 0x10;                         //setting the address to the start of the array

	   for(int j = 0; j< sizeof(name); j++ )        //Reading the array char by char
	   {       
		   Read_data = eeprom_read_byte(Initial_addr);
		   printf_P(PSTR("%c "),Read_data);
    	   Initial_addr++;                           //incrementing the address while reading each char.
		}

	}
}

