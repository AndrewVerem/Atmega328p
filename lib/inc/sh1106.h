
#pragma once
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "TWI.h"
#include "fonts.h"
#include "img.h"

#define DisplayWidth 132
#define DisplayHeight 64

class SH1106 {
	private:
		uint8_t DisplayBuffer[DisplayWidth*DisplayHeight/8];
		uint8_t DisplayCursorX, DisplayCursorY;
		TWI *i2c;
	public:
		SH1106 (TWI *_i2c);
		void SetCursor (uint8_t x, uint8_t y);
		void WriteC (uint8_t command);
		void WriteD (uint8_t dat);
		void DisplayRender (void);
		void DisplayClear (void);
		void DrawPixel (int8_t x, int8_t y);
		void DrawLine (int8_t x1, int8_t y1, int8_t x2, int8_t y2);
		void DrawCircle (int8_t x0, int8_t y0, int8_t radius);
		void DisplayPrintChar (char charCode);
		void DisplayPrint (uint8_t x, uint8_t y, char *str);
		void LoadLogo (void);
		void LoadTemp (void);
		void LoadAtm (void);
		void LoadKus (void);
};