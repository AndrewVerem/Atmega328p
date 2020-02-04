/*
 * TWI.cpp
 *
 * Created: 12.01.2020 23:28:26
 *  Author: AndrewVerem
 */ 

#include "TWI.h"

TWI::TWI (void) {
	TWBR = 0x20;
}

void TWI::TWI_Start (void) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR&(1<<TWINT)));
}

void TWI::TWI_Stop (void) {
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWI::TWI_SendByte(uint8_t byte) {
	TWDR = byte;
	TWCR = (1<<TWINT)|(1<<TWEN); // ¬ключение передачи данных
	while (!(TWCR&(1<<TWINT))); // ќжидание установки TWIN
}

uint8_t TWI::TWI_ReadByte (void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); // ¬ключение приема данных
	while (!(TWCR&(1<<TWINT))); // ќжидание установки TWIN
	return TWDR;
}

uint8_t TWI::TWI_ReadByteLast (void) {
	TWCR = (1<<TWINT)|(1<<TWEN); // ¬ключение приема данных
	while (!(TWCR&(1<<TWINT))); // ќжидание установки TWIN
	return TWDR;
}