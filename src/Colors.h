#pragma once

#include <iostream>
using std::cout;
using std::endl;
#include <cmath>

class Color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
public:
	uint8_t   redInt() const {return red;}
	uint8_t greenInt() const {return green;}
	uint8_t  blueInt() const {return blue;}
	uint8_t alphaInt() const {return alpha;}

	uint8_t   redPremultipliedInt() const {return round((alpha / 255.0f) * red  );}
	uint8_t greenPremultipliedInt() const {return round((alpha / 255.0f) * green);}
	uint8_t  bluePremultipliedInt() const {return round((alpha / 255.0f) * blue );}

	float   redFloat() const {return red   / 255.0f;}
	float greenFloat() const {return green / 255.0f;}
	float  blueFloat() const {return blue  / 255.0f;}
	float alphaFloat() const {return alpha / 255.0f;}

	float   redPremultipliedFloat() const {return (alphaFloat() * red  ) / 255.0f;}
	float greenPremultipliedFloat() const {return (alphaFloat() * green) / 255.0f;}
	float  bluePremultipliedFloat() const {return (alphaFloat() * blue ) / 255.0f;}

	float rPremul32F() const {return (alphaFloat() * red  ) / 255.0f;}
	float gPremul32F() const {return (alphaFloat() * green) / 255.0f;}
	float bPremul32F() const {return (alphaFloat() * blue ) / 255.0f;}

	void setAlpha(float a) {alpha = (uint8_t) (a * 255.0f);};

//	uint32_t packIntoInt() {return alpha << 24 | blue << 16}

	Color(float r, float g, float b, float a = 1.0f) : red((uint8_t) r*255), green((uint8_t) g*255), blue((uint8_t) b*255), alpha ((uint8_t) a*255) {};
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : red(r), green(g), blue(b), alpha(a) {};
	Color() : red {255}, green {0}, blue {255}, alpha {255} {};
	Color(const Color &c) : red(c.redInt()), green(c.greenInt()), blue(c.blueInt()), alpha(c.alphaInt()) {};

};


//packColor
