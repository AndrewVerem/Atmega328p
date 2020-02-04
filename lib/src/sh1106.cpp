
#include "SH1106.h"

SH1106::SH1106 (TWI *_i2c) {
	i2c = _i2c;
	DisplayCursorX = 0;
	DisplayCursorY = 0;		
	WriteC(0xAE);
	WriteC(0x00);
	WriteC(0x10);
	WriteC(0x40);
	WriteC(0xB0);
	WriteC(0x81);
	WriteC(0x80);
	WriteC(0xA1);
	WriteC(0xC8);
	WriteC(0xA6);
	WriteC(0xAD);
	WriteC(0x8B);
	WriteC(0x30);
	WriteC(0xAF);
}

void SH1106::SetCursor (uint8_t x, uint8_t y) {
	DisplayCursorX = x;
	DisplayCursorY = y;
}

void SH1106::WriteC (uint8_t command) {
	i2c->TWI_Start();
	i2c->TWI_SendByte(0x78);
	i2c->TWI_SendByte(0x00);
	i2c->TWI_SendByte(command);
	i2c->TWI_Stop();
}

void SH1106::WriteD (uint8_t dat) {
	i2c->TWI_Start();
	i2c->TWI_SendByte(0x78);
	i2c->TWI_SendByte(0x40);
	i2c->TWI_SendByte(dat);
	i2c->TWI_Stop();
}

void SH1106::DisplayRender (void) {
	uint8_t i, j, page = 0xB0;
	for (i=0; i<DisplayHeight/8; i++) {
		WriteC(page);
		page++;
		for (j=0; j<DisplayWidth; j++) {
			WriteD(DisplayBuffer[i*DisplayWidth + j]);
		}
	}
}

void SH1106::DisplayClear (void) {
	for (uint16_t i=0; i<(DisplayWidth*DisplayHeight/8); i++) {
		DisplayBuffer[i] = 0x00;
	}
}

void SH1106::DrawPixel (int8_t x, int8_t y) {
	DisplayBuffer[x + (y/8)*DisplayWidth] |= (1<<(y&7));
}

void SH1106::DrawLine (int8_t x1, int8_t y1, int8_t x2, int8_t y2) {
	const int8_t deltaX = abs(x2 - x1);
	const int8_t deltaY = abs(y2 - y1);
	const int8_t signX = x1 < x2 ? 1 : -1;
	const int8_t signY = y1 < y2 ? 1 : -1;
	
	int8_t error = deltaX - deltaY;
	DrawPixel(x2, y2);
	while(x1 != x2 || y1 != y2) {
		DrawPixel(x1, y1);
		const int8_t error2 = error*2;
		if (error2 > -deltaY) {
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX) {
			error += deltaX;
			y1 += signY;
		}
	}
}

void SH1106::DrawCircle (int8_t x0, int8_t y0, int8_t radius) {
	int8_t x = 0;
	int8_t y = radius;
	int8_t delta = 1 - 2*radius;
	int8_t error = 0;
	while (y >= 0) {
		DrawPixel(x0 + x, y0 + y);
		DrawPixel(x0 + x, y0 - y);
		DrawPixel(x0 - x, y0 + y);
		DrawPixel(x0 - x, y0 - y);
		error = 2*(delta + y) - 1;
		if (delta < 0 && error <= 0) {
			++x;
			delta += 2*x + 1;
			continue;
		}
		error = 2*(delta - x) - 1;
		if (delta > 0 && error > 0) {
			--y;
			delta += 1 - 2*y;
			continue;
		}
		++x;
		delta += 2*(x - y);
		--y;
	}
}

void SH1106::DisplayPrintChar (char charCode) {
	uint8_t offset = 0, iSymbol = 0, i, j;
	const uint8_t *mas;
	if (charCode >= pgm_read_byte(&FontSymbols[2]) && charCode <= pgm_read_byte(&FontSymbols[2]) + pgm_read_byte(&FontSymbols[3]) - 1) {
		mas = &FontSymbols[0];
	}
	if (charCode >= pgm_read_byte(&FontCyrillicSmall[2]) && charCode <= pgm_read_byte(&FontCyrillicSmall[2]) + pgm_read_byte(&FontCyrillicSmall[3]) - 1) {
		mas = &FontCyrillicSmall[0];
	}
	iSymbol = (uint8_t)charCode - pgm_read_byte(mas+2);
	offset = iSymbol*pgm_read_byte(mas)*pgm_read_byte(mas+1) + 4;
	for (i = DisplayCursorY; i<DisplayCursorY + pgm_read_byte(mas+1); i++) {
		for (j = DisplayCursorX; j<DisplayCursorX + pgm_read_byte(mas); j++) {
			DisplayBuffer[i*DisplayWidth + j] = pgm_read_byte(mas + ((i - DisplayCursorY)*pgm_read_byte(mas) + (j - DisplayCursorX) + offset));
		}
	}
	DisplayCursorX += pgm_read_byte(mas);
}

void SH1106::DisplayPrint (uint8_t x, uint8_t y, char *str) {
	uint8_t i, l = strlen(str);
	SetCursor(x, y);
	for (i=0; i<l; i++) {
		DisplayPrintChar(str[i]);
	}
}

//-----------------------------------------------------------------------------------------------------------------------
void SH1106::LoadLogo (void) {
	for (uint16_t i=0; i<(DisplayWidth*DisplayHeight/8); i++) {
		DisplayBuffer[i] = pgm_read_byte(&duino[i]);
	}
}

void SH1106::LoadTemp (void) {
	for (uint16_t i=0; i<(DisplayWidth*DisplayHeight/8); i++) {
		DisplayBuffer[i] = pgm_read_byte(&temp[i]);
	}
}

void SH1106::LoadAtm (void) {
	for (uint16_t i=0; i<(DisplayWidth*DisplayHeight/8); i++) {
		DisplayBuffer[i] = pgm_read_byte(&atm[i]);
	}
}

void SH1106::LoadKus (void) {
	for (uint16_t i=0; i<(DisplayWidth*DisplayHeight/8); i++) {
		DisplayBuffer[i] = pgm_read_byte(&kus[i]);
	}
}