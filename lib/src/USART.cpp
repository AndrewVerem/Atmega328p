/*
 * USART.cpp
 *
 * Created: 13.01.2020 6:08:11
 *  Author: AndrewVerem
 */

#include "USART.h"

USART::USART (uint16_t baud) {
	uint16_t UBRR = F_CPU/16/baud-1;
	UBRR0H = (uint8_t)(UBRR>>8);
	UBRR0L = (uint8_t) UBRR;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	
}

void USART::TransmitByte (uint8_t data) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART::TransmitStr (char *str) {
	uint8_t i, l = strlen(str);
	for (i=0; i<l; i++) {
		TransmitByte(str[i]);
	}
}