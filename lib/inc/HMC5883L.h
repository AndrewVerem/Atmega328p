

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include <math.h>
#include "TWI.h"

#define AddressWriteHMC5883L 0x3c
#define AddressReadHMC5883L 0x3d
#define ConfRegA 0x00   // Read/Write
#define ConfRegB 0x01   // Read/Write
#define ModeReg 0x02   // Read/Write
#define DataOutX_MSB 0x03   // Read
#define DataOutX_LSB 0x04   // Read
#define DataOutY_MSB 0x05   // Read
#define DataOutY_LSB 0x06   // Read
#define DataOutZ_MSB 0x07   // Read
#define DataOutZ_LSB 0x08   // Read
#define StatusReg 0x09   // Read
#define IndentificationRegA 0x0a   // Read
#define IndentificationRegB 0x0b   // Read
#define IndentificationRegC 0x0c   // Read
#define PI 3.14

class HMC5883L {
	private:
		int16_t X, Y, Z;
		float Heading;
		TWI *i2c;
	public:
		HMC5883L (TWI *twi);
		void MagneticFieldMeasurement (void);
		int16_t getX (void);
		int16_t getY (void);
		int16_t getZ (void);
		float getHeading (void);
};


#endif /* HMC5883L_H_ */