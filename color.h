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

class iRGBA : public Color {
public:
	iRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		m_R = r / 255.0f;
	    m_G = g / 255.0f;
		m_B = b / 255.0f;
		m_A = a / 255.0f;
	}
	iRGBA(unsigned char tone, unsigned char opacity) {
		m_R, m_G, m_B = tone / 255.0f;
		m_A = opacity / 255.0f;
	}
};
class iRGB : public Color {
public:
	iRGB(unsigned char r, unsigned char g, unsigned char b) {
		m_R = r / 255.0f;
		m_G = g / 255.0f;
		m_B = b / 255.0f;
	}
	iRGB(unsigned char tone) {
		m_R, m_G, m_B = tone / 255.0f;
	}
};

class fRGBA : public Color {
public:
	fRGBA(float r, float g, float b, float a) {
		if (r <= 1.0f) {
			if (r > 0.0f) {
				m_R = r;
			}
			else {
				m_R = 0.0f;
			}
		}
		if (g <= 1.0f) {
			if (g > 0.0f) {
				m_G = g;
			}
			else {
				m_G = 0.0f;
			}
		}
		if (b <= 1.0f) {
			if (b > 0.0f) {
				m_B = b;
			}
			else {
				m_B = 0.0f;
			}
		}
		if (a <= 1.0f) {
			if (a > 0.0f) {
				m_A = a;
			}
			else {
				m_A = 0.0f;
			}
		}
	}
	fRGBA(float tone, float opacity) {
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
class fRGB : public Color {
public:
	fRGB(float r, float g, float b) {
		if (r <= 1.0f) {
			if (r > 0.0f) {
				m_R = r;
			}
			else {
				m_R = 0.0f;
			}
		}
		if (g <= 1.0f) {
			if (g > 0.0f) {
				m_G = g;
			}
			else {
				m_G = 0.0f;
			}
		}
		if (b <= 1.0f) {
			if (b > 0.0f) {
				m_B = b;
			}
			else {
				m_B = 0.0f;
			}
		}
	}
	fRGB(float tone) {
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