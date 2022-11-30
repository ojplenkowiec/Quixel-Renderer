#include "shadermanager.h"

ShaderManager::ShaderManager()
{
	m_Shaders = new std::vector<Shader>();
	m_Shaders->push_back(Shader("InstancedAABBs.shader"));
	// push_back any future shaders!

	m_CurrentlyBoundShader = Null;
}

ShaderManager::~ShaderManager()
{
	delete m_Shaders;
}

void ShaderManager::Bind(ShaderType type)
{
	if (!type == m_CurrentlyBoundShader) {
		m_Shaders->at(type).Bind();
		m_CurrentlyBoundShader = type;
	}
}

void ShaderManager::UpdateViewMatrix(glm::mat4 viewMatrix)
{

}

void ShaderManager::UpdateProjectionMatrix(glm::mat4 projMatrix)
{
}
