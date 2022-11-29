#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"
#include "glm.hpp"

class Sprite {
private:
	Texture m_Texture;
	float m_Width;
	float m_Height;
	glm::vec2 m_Position;
	glm::mat2 m_TransformationMatrix;
public:
	Sprite();
	~Sprite();
};

#endif