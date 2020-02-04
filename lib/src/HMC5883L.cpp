

#include "HMC5883L.h"

HMC5883L::HMC5883L (TWI *twi) {
	i2c = twi;
	i2c->TWI_Start();
	i2c->TWI_SendByte(AddressWriteHMC5883L);
	i2c->TWI_SendByte(ConfRegA);
	i2c->TWI_SendByte(0x70);
	i2c->TWI_Stop();
	i2c->TWI_Start();
	i2c->TWI_SendByte(AddressWriteHMC5883L);
	i2c->TWI_SendByte(ConfRegB);
	i2c->TWI_SendByte(0xa0);
	i2c->TWI_Stop();
}

void HMC5883L::MagneticFieldMeasurement (void) {
	i2c->TWI_Start();
	i2c->TWI_SendByte(AddressWriteHMC5883L);
	i2c->TWI_SendByte(ModeReg);
	i2c->TWI_SendByte(0x01);
	i2c->TWI_Stop();
	i2c->TWI_Start();
	i2c->TWI_SendByte(AddressWriteHMC5883L);
	i2c->TWI_SendByte(DataOutX_MSB);
	i2c->TWI_Start();
	i2c->TWI_SendByte(AddressReadHMC5883L);
	X = i2c->TWI_ReadByte();
	X <<= 8;
	X |= i2c->TWI_ReadByte();
	Y = i2c->TWI_ReadByte();
	Y <<= 8;
	Y |= i2c->TWI_ReadByte();
	Z = i2c->TWI_ReadByte();
	Z <<= 8;
	Z |= i2c->TWI_ReadByteLast();
	i2c->TWI_Stop();
	Heading = atan2(Y, X);
	if (Heading < 0) Heading += 2*PI;
	if (Heading > 2*PI) Heading -= 2*PI;
	Heading = (Heading*180)/PI;
}

int16_t HMC5883L::getX (void) {
	return X;
}

int16_t HMC5883L::getY (void) {
	return Y;
}

int16_t HMC5883L::getZ (void) {
	return Z;
}

float HMC5883L::getHeading (void) {
	return Heading;
}