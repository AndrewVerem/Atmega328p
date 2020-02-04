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
	TWCR = (1<<TWINT)|(1<<TWEN); // ��������� �������� ������
	while (!(TWCR&(1<<TWINT))); // �������� ��������� TWIN
}

uint8_t TWI::TWI_ReadByte (void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); // ��������� ������ ������
	while (!(TWCR&(1<<TWINT))); // �������� ��������� TWIN
	return TWDR;
}

uint8_t TWI::TWI_ReadByteLast (void) {
	TWCR = (1<<TWINT)|(1<<TWEN); // ��������� ������ ������
	while (!(TWCR&(1<<TWINT))); // �������� ��������� TWIN
	return TWDR;
}