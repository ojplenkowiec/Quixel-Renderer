#define GLEW_STATIC
#include <glew.h>

#include <glfw3.h>

#include "debugging.h"

#include "callbacks.h"
#include "inputstate.h"
#include "globals.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

int main() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("Couldn't initialize GLFW!");
	}

	/* Window Creation -----------------------------------------*/

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Quixel Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Couldn't initialize GLEW!");
	}

	glfwSwapInterval(1);

	/* GLFW Callbacks --------------------------------------------*/

	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);

	/* Buffer Data ---------------------------------------------*/

	const float quadVertices[] = {
		 0.1f,  0.1f,			// TR
		 0.1f, -0.1f,			// BR
		-0.1f, -0.1f,			// BL
		-0.1f,  0.1f			// TL
	};

	const unsigned int quadIndexes[] = {
		0, 1, 2, 0, 2, 3
	};

	/* Buffer Creation -------------------------------*/

	VertexBuffer quadVertexBuffer = VertexBuffer(quadVertices, 8 * sizeof(float));

	VertexBufferLayout quadVertexBufferLayout = VertexBufferLayout();
	quadVertexBufferLayout.Push<float>(2);

	IndexBuffer quadIndexBuffer = IndexBuffer(quadIndexes, 6);

	/* VAO Creation ----------------------------------*/

	VertexArray quadVertexArray = VertexArray(quadVertexBuffer, quadVertexBufferLayout);

	Shader basicShader = Shader("basic.shader");

	Renderer renderer = Renderer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (glfwWindowShouldClose(window) != GLFW_TRUE) {
		renderer.Clear();
		renderer.Draw(quadVertexArray, quadIndexBuffer, basicShader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}