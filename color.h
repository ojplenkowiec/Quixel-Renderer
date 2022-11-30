#ifndef COLOR_H
#define COLOR_H

#include "debugging.h"
#include "glm.hpp"

class Color {
protected:
	Color() : m_R(1.0f), m_G(1.0f), m_B(1.0f), m_A(1.0f) {}
	float m_R;
	float m_G;
	float m_B;
	float m_A;
public:
	inline float R() { return m_R; }
	inline float G() { return m_G; }
	inline float B() { return m_B; }
	inline float A() { return m_A; }
	inline glm::vec4 Data() { return glm::vec4(m_R, m_G, m_B, m_A); }
};

class HexRGBA : public Color {
public:
	HexRGBA(unsigned char rHex, unsigned char gHex, unsigned char bHex, unsigned char aHex) {
		m_R = rHex / 255.0f;
	    m_G = gHex / 255.0f;
		m_B = bHex / 255.0f;
		m_A = aHex / 255.0f;
	}
	HexRGBA(unsigned char toneHex, unsigned char opacityHex) {
		m_R, m_G, m_B = toneHex / 255.0f;
		m_A = opacityHex / 255.0f;
	}
};
class HexRGB : public Color {
public:
	HexRGB(unsigned char rHex, unsigned char gHex, unsigned char bHex) {
		m_R = rHex / 255.0f;
		m_G = gHex / 255.0f;
		m_B = bHex / 255.0f;
	}
	HexRGB(unsigned char toneHex) {
		m_R, m_G, m_B = toneHex / 255.0f;
	}
};

class RGBA : public Color {
public:
	RGBA(float rVal, float gVal, float bVal, float aVal) {
		if (rVal <= 1.0f) {
			if (rVal > 0.0f) {
				m_R = rVal;
			}
			else {
				m_R = 0.0f;
			}
		}
		if (gVal <= 1.0f) {
			if (gVal > 0.0f) {
				m_G = gVal;
			}
			else {
				m_G = 0.0f;
			}
		}
		if (bVal <= 1.0f) {
			if (bVal > 0.0f) {
				m_B = bVal;
			}
			else {
				m_B = 0.0f;
			}
		}
		if (aVal <= 1.0f) {
			if (aVal > 0.0f) {
				m_A = aVal;
			}
			else {
				m_A = 0.0f;
			}
		}
	}
	RGBA(float tone, float opacity) {
		if (tone <= 1.0f) {
			if (tone > 0.0f) {
				m_R = tone;
				m_G = tone;
				m_B = tone;
			}
			else {
				m_R = 0.0f;
				m_G = 0.0f;
				m_B = 0.0f;
			}
		}
		if (opacity <= 1.0f) {
			if (opacity > 0.0f) {
				m_A = opacity;
			}
			else {
				m_A = 0.0f;
			}
		}
	}
};
class RGB : public Color {
public:
	RGB(float rVal, float gVal, float bVal) {
		if (rVal <= 1.0f) {
			if (rVal > 0.0f) {
				m_R = rVal;
			}
			else {
				m_R = 0.0f;
			}
		}
		if (gVal <= 1.0f) {
			if (gVal > 0.0f) {
				m_G = gVal;
			}
			else {
				m_G = 0.0f;
			}
		}
		if (bVal <= 1.0f) {
			if (bVal > 0.0f) {
				m_B = bVal;
			}
			else {
				m_B = 0.0f;
			}
		}
	}
	RGB(float tone) {
		if (tone <= 1.0f) {
			if (tone > 0.0f) {
				m_R = tone;
				m_G = tone;
				m_B = tone;
			}
			else {
				m_R = 0.0f;
				m_G = 0.0f;
				m_B = 0.0f;
			}
		}
	}
};

#endif