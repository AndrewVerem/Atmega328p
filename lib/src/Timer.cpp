/*
 * PWM.cpp
 *
 * Created: 15.01.2020 22:11:35
 *  Author: Veremeev Andrey
 */ 

#include "Timer.h"

void InitTimer1 (void) {
	TCCR1B |= (1<<WGM12)|(1<<CS02)|(1<<CS00);
	TIMSK1 |= 1<<OCIE1A;
	OCR1AH = 0x3d;
	OCR1AL = 0x09;
}

void InitTimer2 (void) {
	DDRB |= (1<<3);
	PORTB &= ~(1<<3);
	TCCR2A |= (1<<COM2A1)|(1<<WGM21)|(1<<WGM20);
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2A = 0x18;
}

long Scaling (long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min)*(out_max - out_min)/(in_max - in_min) + out_min;
}