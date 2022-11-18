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
#include "main.h"

int main() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("Couldn't initialize GLFW!");
	}

	/* Window Creation -----------------------------------------*/

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Quixel Engine", monitor, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Couldn't initialize GLEW!");
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	/* GLFW Callbacks --------------------------------------------*/

	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, glfwMousePositionCallback);

	/* Buffer Data ---------------------------------------------*/

	const float cubeVertices[] = { // 24
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	unsigned int cubeIndexes[] = { // 36
		0, 3, 2,
		0, 2, 1,
		6, 7, 4,
		6, 4, 5,
		2, 6, 1,
		1, 6, 5,
		7, 3, 4,
		4, 3, 0,
		7, 6, 3,
		6, 2, 3,
		0, 1, 4,
		1, 5, 4
	};

	/* Buffer Creation -------------------------------*/

	VertexBuffer cubeVertexBuffer = VertexBuffer(cubeVertices, 24 * sizeof(float));
	VertexBufferLayout cubeVertexBufferLayout = VertexBufferLayout();
	cubeVertexBufferLayout.Push<float>(3);
	IndexBuffer cubeIndexBuffer = IndexBuffer(cubeIndexes, 36);

	/* VAO Creation ----------------------------------*/

	VertexArray cubeVertexArray = VertexArray(cubeVertexBuffer, cubeVertexBufferLayout);
	Shader basicShader = Shader("basic.shader");

	/* Renderer instantiation --------------------------*/

	Renderer renderer = Renderer(1, mode->width / (float)mode->height);

	/* Color uniform data ------------------------------------------------------------*/

	glm::vec4 cubeColor = glm::vec4(1.0f, 0.75f, 0.75f, 1.0f);

	/* Camera vector data -------------------------------*/

	float mouseSensitivity = 0.1f;

	float cameraSpeed = 4.0f;
	float rotateSpeed = 90.0f;

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.15f, 0.25f, 1.0f);
	while (glfwWindowShouldClose(window) != GLFW_TRUE) {
		renderer.Clear();
		updateDeltaTime(); // updates DELTA_TIME global

		basicShader.Bind();

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		basicShader.SetUniformMat4f("u_model", modelMatrix);

		renderer.Draw(cubeVertexArray, cubeIndexBuffer, basicShader);

		glfwSwapBuffers(window);

		/* Keyboard updates --------------------------*/

		if (INPUT_STATE >= SHIFT_KEY) {
			cameraSpeed = 8.0f;
		}
		else if (INPUT_STATE >= CTRL_KEY) {
			cameraSpeed = 2.0f;
		}
		else {
			cameraSpeed = 4.0f;
		}

		if (!(INPUT_STATE >= W_KEY + S_KEY)) {
			if (INPUT_STATE >= W_KEY) {
				renderer.r_camera.TranslateForwards(cameraSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= S_KEY) {
				renderer.r_camera.TranslateForwards(-cameraSpeed * g_DELTA_TIME);
			}
		}
		if (!(INPUT_STATE >= A_KEY + D_KEY)) {
			if (INPUT_STATE >= A_KEY) {
				renderer.r_camera.TranslateRight(-cameraSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= D_KEY) {
				renderer.r_camera.TranslateRight(cameraSpeed * g_DELTA_TIME);
			}
		}

		if (!(INPUT_STATE >= LEFT_KEY + RIGHT_KEY)) {
			if (INPUT_STATE >= LEFT_KEY) {
				renderer.r_camera.Rotate(0.0f, -rotateSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= RIGHT_KEY) {
				renderer.r_camera.Rotate(0.0f, rotateSpeed * g_DELTA_TIME);
			}
		}
		if (!(INPUT_STATE >= UP_KEY + DOWN_KEY)) {
			if (INPUT_STATE >= UP_KEY) {
				renderer.r_camera.Rotate(rotateSpeed * g_DELTA_TIME, 0.0f);
			}
			else if (INPUT_STATE >= DOWN_KEY) {
				renderer.r_camera.Rotate(-rotateSpeed * g_DELTA_TIME, 0.0f);
			}
		}

		if (INPUT_STATE >= ESC_KEY) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		/* Mouse updates ----------------------------------------------*/ //INPUTS CLASS FOR RESETTING GLOBAL VARIABLES ETC NEEDED//

		renderer.r_camera.Rotate(-g_MOUSE_Y_DIFFERENCE * mouseSensitivity, g_MOUSE_X_DIFFERENCE * mouseSensitivity);
		g_MOUSE_X_DIFFERENCE = 0.0;
		g_MOUSE_Y_DIFFERENCE = 0.0;

		/* Poll events --------------------------------------*/

		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}