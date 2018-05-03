#pragma once

class Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;

	uint8_t   redInt() {return red;}
	uint8_t greenInt() {return green;}
	uint8_t  blueInt() {return blue;}
	uint8_t alphaInt() {return alpha;}

	float   redFloat() {return red   / 256.0f;}
	float greenFloat() {return green / 256.0f;}
	float  blueFloat() {return blue  / 256.0f;}
	float alphaFloat() {return alpha / 256.0f;}

//	uint32_t packIntoInt() {return alpha << 24 | blue << 16}
};


//packColor
