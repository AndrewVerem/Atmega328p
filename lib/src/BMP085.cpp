/*
 * BMP085.cpp
 *
 * Created: 17.01.2020 15:18:09
 *  Author: Andrey Veremeev
 */ 

#include "BMP085.h"

BMP085::BMP085 (TWI *_i2c) {
	i2c = _i2c;
	sealevelPressure = 101325;
	AC1 = ReadS16(BMP085_EEPROM_AC1);
	AC2 = ReadS16(BMP085_EEPROM_AC2);
	AC3 = ReadS16(BMP085_EEPROM_AC3);
	AC4 = ReadU16(BMP085_EEPROM_AC4);
	AC5 = ReadU16(BMP085_EEPROM_AC5);
	AC6 = ReadU16(BMP085_EEPROM_AC6);
	B1 = ReadS16(BMP085_EEPROM_B1);
	B2 = ReadS16(BMP085_EEPROM_B2);
	MB = ReadS16(BMP085_EEPROM_MB);
	MC = ReadS16(BMP085_EEPROM_MC);
	MD = ReadS16(BMP085_EEPROM_MD);
}

short BMP085::ReadS16(unsigned char address) {
	short result;
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressWrite);
	i2c->TWI_SendByte(address);
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressRead);
	result = i2c->TWI_ReadByte();
	result <<= 8;
	result |= i2c->TWI_ReadByteLast();
	i2c->TWI_Stop();
	return result;
}

unsigned short BMP085::ReadU16(unsigned char address) {
	unsigned short result;
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressWrite);
	i2c->TWI_SendByte(address);
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressRead);
	result = i2c->TWI_ReadByte();
	result <<= 8;
	result |= i2c->TWI_ReadByteLast();
	i2c->TWI_Stop();
	return result;
}

long BMP085::ReadUT (void) {
	long result;
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressWrite);
	i2c->TWI_SendByte(BMP085_ControlRegister);
	i2c->TWI_SendByte(BMP085_StartMeasurementUT);
	i2c->TWI_Stop();
	_delay_ms(5);
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressWrite);
	i2c->TWI_SendByte(BMP085_RegisterADC);
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressRead);
	result = i2c->TWI_ReadByte();
	result <<= 8;
	result |= i2c->TWI_ReadByteLast();
	i2c->TWI_Stop();
	return result;
}

long BMP085::ReadUP (void) {
	long result;
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressWrite);
	i2c->TWI_SendByte(BMP085_ControlRegister);
	i2c->TWI_SendByte(BMP085_StartMeasurementUP + (oversampling<<6));
	i2c->TWI_Stop();
	_delay_ms(5);
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressWrite);
	i2c->TWI_SendByte(BMP085_RegisterADC);
	i2c->TWI_Start();
	i2c->TWI_SendByte(BMP085_DeviceAddressRead);
	result = i2c->TWI_ReadByte();
	result <<= 8;
	result |= i2c->TWI_ReadByte();
	result <<= 8;
	result |= i2c->TWI_ReadByteLast();
	result >>= (8 - oversampling);
	i2c->TWI_Stop();
	return result;
}

void BMP085::Measurement (void) {
	UT = ReadUT();
	X1 = (UT - (long)AC6)*((long)AC5)>>15;
	X2 = ((long)MC<<11)/(X1 + (long)MD);
	B5 = X1 + X2;
	T = (B5 + 8)>>4;
	T /= 10;
	UP = ReadUP();
	B6 = B5 - 4000;
	X1 = ((long)B2*((B6*B6)>>12))>>11;
	X2 = ((long)AC2*B6)>>11;
	X3 = X1 + X2;
	B3 = ((((long)AC1*4 + X3)<<oversampling) + 2)/4;
	X1 = ((long)AC3*B6)>>13;
	X2 = ((long)B1*((B6*B6)>>12))>>16;
	X3 = ((X1 + X2) + 2)>>2;
	B4 = ((unsigned long)AC4*(unsigned long)(X3 + 32768))>>15;
	B7 = ((unsigned long)UP - B3)*(unsigned long)(50000UL>>oversampling);
	if (B7 < 0x80000000) {
		P = (B7*2)/B4;
		}else{
		P = (B7/B4)*2;
	}
	X1 = (P>>8)*(P>>8);
	X1 = (X1*3038)>>16;
	X2 = (-7357*P)>>16;
	P = P + ((X1 + X2 + (long)3791)>>4);
	altitude = 44330*(1.0 - pow((float)P/sealevelPressure, 0.1903));
}

float BMP085::getTemperature (void) {
	return T;
}

long BMP085::getAtmospherePressure (void) {
	return P;
}

float BMP085::getAltitude (void) {
	return altitude;
}