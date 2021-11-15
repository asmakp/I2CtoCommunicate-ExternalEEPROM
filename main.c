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

	//-------------------------------- VG DELUPPGIFT PART 1 AND 2------------------------------

	//variables to write the data

	/*uint8_t *array[] = {"Happy   ", "new     ", "year    "};
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
		
		uint8_t *ch;
		ch = array[i];
		eeprom_write_page(Initial_addr, ch);

		Initial_addr += Pagesize; // incrementing the page size ,pagesize = 8;
	}

	

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

	//To print the hexadecimal values of the chars printed above
	printf_P(PSTR("\n My HEX  string  \n"));
	for (int i = 0; i < Size_of_array - 1; i++)
	{
		printf_P(PSTR("%X "), Read_string[i]);
	}*/

	//----------------------VG DELUPPGIFT PART 3 ---------------------------------------

	uint8_t New_array[] = "Here i am studying IOT program";
	uint8_t len_of_string = sizeof(New_array);
	uint8_t No_of_pages = len_of_string / Pagesize;
	uint8_t Initial_addr = 0x10;

	printf_P(PSTR("size of new_array= %d\n"), sizeof(New_array));					// length is 31 as per the New_array declared above
	printf_P(PSTR("excess char (out of boundry chars) = %d\n"), len_of_string % 8); // 7 excess chars
	printf_P(PSTR("No of pages= %d\n"), No_of_pages);								//   31/8 = 3

	uint8_t excess_char = len_of_string % 8; //gives the number of chars which are more than page size

	for (int i = 0; i < No_of_pages; i++)
	{
		uint8_t *ch;
		ch = &New_array[i * Pagesize]; //assigning the address to the pointer
		eeprom_write_page(Initial_addr, ch);
		Initial_addr += Pagesize;
	}

	for (int i = (len_of_string - excess_char); i < len_of_string; i++) // writing the out of boundary chars
	{
		eeprom_write_byte(Initial_addr, New_array[i]);
		Initial_addr++;
	}

	//variables to read the data
	uint8_t Read_string[len_of_string]; // array to read the chars

	Initial_addr = 0x10; //re-initializing the start address

	eeprom_sequential_read(Read_string, Initial_addr, len_of_string);

	//To print the string the text
	printf_P(PSTR("\n My string in  ASCII char \n"));
	for (int i = 0; i < len_of_string - 1; i++)
	{
		printf_P(PSTR("%c  "), Read_string[i]);
	}

	//To print the hexadecimal values of the chars printed above
	printf_P(PSTR("\n My HEX  string  \n"));
	for (int i = 0; i < len_of_string - 1; i++)
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
