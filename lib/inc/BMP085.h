/*
 * BMP085.h
 *
 * Created: 17.01.2020 15:16:39
 *  Author: Andrey Veremeev
 */ 

#ifndef BMP085_H_
#define BMP085_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include "TWI.h"

#define BMP085_DeviceAddressRead 0xEF
#define BMP085_DeviceAddressWrite 0xEE
#define BMP085_ControlRegister 0xF4
#define BMP085_StartMeasurementUT 0x2E
#define BMP085_StartMeasurementUP 0x34
#define BMP085_RegisterADC 0xF6
#define BMP085_EEPROM_AC1 0xAA
#define BMP085_EEPROM_AC2 0xAC
#define BMP085_EEPROM_AC3 0xAE
#define BMP085_EEPROM_AC4 0xB0
#define BMP085_EEPROM_AC5 0xB2
#define BMP085_EEPROM_AC6 0xB4
#define BMP085_EEPROM_B1 0xB6
#define BMP085_EEPROM_B2 0xB8
#define BMP085_EEPROM_MB 0xBA
#define BMP085_EEPROM_MC 0xBC
#define BMP085_EEPROM_MD 0xBE
#define oversampling 0x00

#define mmHg 0.007500637554192

class BMP085 {
	private:
		short AC1, AC2, AC3, B1, B2, MB, MC, MD;
		unsigned short AC4, AC5, AC6;
		long UT, UP, X1, X2, X3, B3, B5, B6, P;
		unsigned long B4, B7;
		float T, sealevelPressure, altitude;
		TWI *i2c;
	public:
		BMP085 (TWI *_i2c);
		short ReadS16 (unsigned char address);
		unsigned short ReadU16 (unsigned char address);
		long ReadUT(void);
		long ReadUP (void);
		void Measurement (void);
		float getTemperature (void);
		long getAtmospherePressure (void);
		float getAltitude (void);
};

#endif /* BMP085_H_ */