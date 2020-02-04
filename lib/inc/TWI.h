/*
 * TWI.h
 *
 * Created: 12.01.2020 23:27:39
 *  Author: AndrewVerem
 */ 


#pragma once
#include <avr/io.h>

class TWI {
	public:
		TWI (void);
		void TWI_Start (void);
		void TWI_Stop (void);
		void TWI_SendByte(uint8_t byte);
		uint8_t TWI_ReadByte (void);
		uint8_t TWI_ReadByteLast (void);
};