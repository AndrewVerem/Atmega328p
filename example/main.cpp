/*
 * Display_SH1106.cpp
 *
 * Created: 29.12.2019 19:26:51
 * Author : AndrewVerem
 */


#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "USART.h"
#include "TWI.h"
#include "SH1106.h"
#include "Timer.h"
#include "BMP085.h"
#include "HMC5883L.h"

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
uint8_t second = 0, minute = 27, hour = 16;

ISR (TIMER1_COMPA_vect) {
	if (second == 59) {
		second = 0;
		if (minute == 59) {
			minute = 0;
			if (hour == 23) {
				hour = 0;
			}else{
				hour++;
			}
		}else{
			minute++;
		}
	}else{
		second++;
	}
}

int main(void)
{
	char buf[5] = "";
	char timeStr[8] = "";
	char buffer[15] = "";
	//uint8_t angle=0, value = 0;
	
	USART usart(9600);
	TWI i2c;
	SH1106 display(&i2c);
	BMP085 bmp(&i2c);
	HMC5883L magnetic(&i2c);
	InitTimer1();
	InitTimer2();
	sei();
	
	DDRD &= ~(1<<7);
	PORTD |= 1<<7;
	float x, y, f, deg;
	int8_t page = 0;
	display.DisplayClear();
	display.DisplayRender();
    while (1) 
    { 
		if (!((1<<7)&PIND)) {
			if (page == 5) {
				page = 0;
			}else{
				page++;
			}
		}
		if (page == 0) {
			display.LoadLogo();
		}
		if (page == 1) {
			deg = hour*30 - 90;
			f = (deg*3.14)/(float)180;
			x = (float)66 + 15*cos(f);
			y = (float)31 + 15*sin(f);
			display.DrawLine(66, 31, round(x), round(y));
			deg = minute*6 - 90;
			f = (deg*3.14)/(float)180;
			x = (float)66 + 25*cos(f);
			y = (float)31 + 25*sin(f);
			display.DrawLine(66, 31, round(x), round(y));
			deg = second*6 - 90;
			f = (deg*3.14)/(float)180;
			x = (float)66 + 25*cos(f);
			y = (float)31 + 25*sin(f);
			display.DrawCircle(66, 31, 30);
			display.DrawLine(66, 31, round(x), round(y));
			display.DrawCircle(round(x), round(y), 2);
			strcpy(timeStr, "");
			if (hour < 10) {
				strcat(timeStr, "0");
			}
			itoa(hour, buf, 10);
			strcat(timeStr, buf);
			strcat(timeStr, ":");
			if (minute < 10) {
				strcat(timeStr, "0");
			}
			itoa(minute, buf, 10);
			strcat(timeStr, buf);
			strcat(timeStr, ":");
			if (second < 10) {
				strcat(timeStr, "0");
			}
			itoa(second, buf, 10);
			strcat(timeStr, buf);
			display.DisplayPrint(40, 7, timeStr);
		}
		if (page == 2) {
			bmp.Measurement();
			strcpy(buffer, "");
			strcat(buffer, "температура: ");
			strcat(buffer, dtostrf(bmp.getTemperature(), 5, 1, buf));
			display.DisplayPrint(5, 3, buffer);
			strcpy(buffer, "");
			strcat(buffer, "атм. давление: ");
			strcat(buffer, itoa(bmp.getAtmospherePressure()*mmHg, buf, 10));
			display.DisplayPrint(5, 4, buffer);
			strcpy(buffer, "");
			strcat(buffer, "высота: ");
			strcat(buffer, itoa(bmp.getAltitude(), buf, 10));
			display.DisplayPrint(5, 5, buffer);
		}
		if (page == 3) {
			magnetic.MagneticFieldMeasurement();
			display.DisplayPrint(30, 1, "магнитометр:");
			display.DisplayPrint(5, 3, itoa(magnetic.getX(), buf, 10));
			display.DisplayPrint(5, 4, itoa(magnetic.getY(), buf, 10));
			display.DisplayPrint(5, 5, itoa(magnetic.getZ(), buf, 10));
			display.DisplayPrint(5, 6, itoa(magnetic.getHeading(), buf, 10));
		}
		if (page == 4) {
			display.LoadKus();
		}
		if (page == 5) {
			//for (angle=0; angle<180; angle++) {
			//	value = Scaling(angle, 0, 180, SERVO_MIN, SERVO_MAX);
			//	OCR2A = value;
			//	_delay_ms(5);
			//}
			//for (angle=180; angle>0; angle--) {
			//	value = Scaling(angle, 0, 180, SERVO_MIN, SERVO_MAX);
			//	OCR2A = value;
			//	_delay_ms(5);
			//}
		}
		display.DisplayRender();
		display.DisplayClear();
    }
}

