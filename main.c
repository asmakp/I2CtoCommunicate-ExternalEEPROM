//Asma Khalid Patel
//IOT 2020
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

void main(void)
{

	i2c_init();
	uart_init();

	sei();

	uint8_t Eeprom_data;

	// DELUPPGIFT 1: KONFIGURERA OCH ANVÄND I 2C GENERELLT
	/*printf_P(PSTR("Manufature CODE: "));
	Eeprom_data = eeprom_read_byte(0xFA);
	printf_P(PSTR("%x\n"), Eeprom_data);

	printf_P(PSTR("Device CODE: "));
	Eeprom_data = eeprom_read_byte(0xFB);
	printf_P(PSTR("%x\n"), Eeprom_data);

	printf_P(PSTR("32-bit Serial Number\n: "));
	Eeprom_data = eeprom_read_byte(0xFC);
	printf_P(PSTR("%x\n"), Eeprom_data);

	Eeprom_data = eeprom_read_byte(0xFD);
	printf_P(PSTR("%x\n"), Eeprom_data);

	Eeprom_data = eeprom_read_byte(0xFE);
	printf_P(PSTR("%x\n"), Eeprom_data);

	Eeprom_data = eeprom_read_byte(0xFF);
	printf_P(PSTR("%x\n"), Eeprom_data);*/

	/* Variables for DELUPPGIFT 2
	uint8_t Read_data;
	char name[] = "Asma";
	uint8_t Initial_addr = 0x10;*/

	//variables to write the data

	uint8_t *array[] = {"Happy   ", "new     ", "year    "};
	uint8_t Initial_addr = 0x10;

	uint8_t Pages = sizeof(array) / sizeof(array[0]);
	uint8_t Size_of_array = strlen(array[0]) * Pages;
	uint8_t Str_len = strlen(array[0]);

	//printf_P(PSTR("NO of Pages = %d\n"), Pages);						  // NO of Pages = 7
	//printf_P(PSTR("sizeof(array[0]) = %d\n"), sizeof(array[0]));		  //sizeof(array) = 2
	//printf_P(PSTR("length of string,pagesize = %d\n"), strlen(array[0])); //length of string,pagesize = 8
	//printf_P(PSTR("length of whole array = %d\n"), Size_of_array);		  //length of whole array = 56

	//WRITING TO A PAGE

	for (int i = 0; i < Pages; i++) // NO of Pages = 7
	{
		//char *ch;
		uint8_t *ch;
		ch = array[i];
		eeprom_write_page(Initial_addr, ch);

		//	printf_P(PSTR("%x\n"), Initial_addr);

		Initial_addr += Pagesize; //pagesize = 8;
	}

	//printf_P(PSTR("\n readstring\n"));

	//variables to read the data
	uint8_t Read_string[Str_len * Pages];

	Initial_addr = 0x10;

	//SEQUENTIAL READ FUNCTION

	eeprom_sequential_read(Read_string, Initial_addr, Size_of_array);

	//To print the string the text
	printf_P(PSTR("\n My string in  ASCII char \n"));
	for (int i = 0; i < Size_of_array - 1; i++)
	{
		printf_P(PSTR("%c  "), Read_string[i]);
	}

	//To print the hexadecimal values of the chars pri nted above
	printf_P(PSTR("\n My HEX  string  \n"));
	for (int i = 0; i < Size_of_array - 1; i++)
	{
		printf_P(PSTR("%X "), Read_string[i]);
	}

	while (1)
	{

		/*/DELUPPGIFT 2: IMPLEMENTERA OCH DEMONSTRERA EEPROMKOMMUNIKATION
	for (int i = 0; i < sizeof(name); i++) //to loop until all the characters in the array are written
	{
		eeprom_write_byte(Initial_addr, name[i]); //send each char
		Initial_addr++;							  //increment address to acomodate each char
	}
	// _delay_ms(100);
	Initial_addr = 0x10; //setting the address to the start of the array

	for (int j = 0; j < sizeof(name); j++) //Reading the array char by char
	{
		Read_data = eeprom_read_byte(Initial_addr);
		printf_P(PSTR("%c"), Read_data);
		Initial_addr++; //incrementing the address while reading each char.
	}*/
	}
}
