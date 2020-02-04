/*
 * USART.h
 *
 * Created: 13.01.2020 6:03:52
 *  Author: AndrewVerem
 */ 


#ifndef USART_H_
#define USART_H_

#pragma once
#define F_CPU 16000000UL
#include <avr/io.h>
#include <string.h>

class USART {
	public:
		USART (uint16_t baud);
		void TransmitByte (uint8_t data);
		void TransmitStr (char *str);
};



#endif /* USART_H_ */