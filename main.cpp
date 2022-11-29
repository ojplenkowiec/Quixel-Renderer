#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "debugging.h"

#include "callbacks.h"
#include "inputstate.h"
#include "window.h"
#include "engine.h"
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
#include <random>

int main() {
	// glfwSetErrorCallback(glfwErrorCallback);
	// glfwSetWindowCloseCallback(gameWindow.GetID(), glfwWindowCloseCallback);
	// glfwSetKeyCallback(gameWindow.GetID(), glfwKeyCallback);
	// glfwSetMouseButtonCallback(gameWindow.GetID(), glfwMouseButtonCallback);
	// glfwSetCursorPosCallback(gameWindow.GetID(), glfwMousePositionCallback);

	/* Buffer Creation -------------------------------*/

	/* VAO Creation ----------------------------------*/

	/* Shader Creation --------------------------*/

	/* Renderer instantiation --------------------------*/

	Engine engine = Engine(800, 600, "QUACK!");
	engine.SetClearColorHex(150, 150, 150, 255);

	while (!engine.ShouldClose()) {
		engine.Clear();

		// render stuff here

		engine.Update();
	}
	engine.Terminate();
}
