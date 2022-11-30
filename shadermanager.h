#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shader.h"
#include <vector>
#include "glm.hpp"

enum ShaderType : int32_t {
	Null = -1,
	InstancedAABBs = 0
};

class ShaderManager {
private:
	std::vector<Shader>* m_Shaders;
	ShaderType m_CurrentlyBoundShader;
public:
	ShaderManager();
	~ShaderManager();

	void Bind(ShaderType type);
	void UpdateViewMatrix(glm::mat4 viewMatrix);
	void UpdateProjectionMatrix(glm::mat4 projMatrix);
};

#endif