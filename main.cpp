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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	// glfwWindowHint(GLFW_SAMPLES, 4);

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

	glfwSwapInterval(0);

	/* GLFW Callbacks --------------------------------------------*/

	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, glfwMousePositionCallback);

	/* Buffer Data ---------------------------------------------*/

	const float cubeVertices[] = { // 24
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};

	const unsigned int cubeIndexes[] = { // 36
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

	Renderer renderer = Renderer();

	/* Matrix uniform data -------------------------------------*/

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(70.0f), (float)mode->width / (float)mode->height, 0.1f, 1000.0f);

	/* Color uniform data ------------------------------------------------------------*/

	glm::vec4 cubeColor = glm::vec4(1.0f, 0.75f, 0.75f, 1.0f);

	/* Camera vector data -------------------------------*/

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float pitch = 0.0f;
	float yaw = -90.0f;

	float mouseSensitivity = 0.1f;

	float cameraSpeed = 2.0f;

	/* Time variables ----------------------------*/

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
	float currentFrameTime = 0.0f;

	/* Main loop ------------------------------------------*/
	
	const unsigned int numberOfCubes = 2000;
	glm::vec3 cubePositions[numberOfCubes]{};
	for (int i = 0; i < numberOfCubes; i++) {
		for (int j = 0; j < 3; j++) {
			cubePositions[i][j] = rand() % 1000 / 50.0f - 25.0f;
		}
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.05f, 0.15f, 0.25f, 1.0f);
	while (glfwWindowShouldClose(window) != GLFW_TRUE) {
		renderer.Clear();
		glClear(GL_DEPTH_BUFFER_BIT);

		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		cameraUp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch + 90.0f));
		cameraUp.y = sin(glm::radians(pitch + 90.0f));
		cameraUp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch + 90.0f));

		basicShader.Bind();

		viewMatrix = glm::lookAt(cameraPos, cameraPos + glm::normalize(cameraDirection), cameraUp);
		for (unsigned int i = 0; i < numberOfCubes; i++) { // 200 cube positions!
			float angle = 20.0f * i;
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05, 0.05, 30.0));

			basicShader.SetUniformMat4f("u_model", modelMatrix);
			basicShader.SetUniformMat4f("u_view", viewMatrix);
			basicShader.SetUniformMat4f("u_projection", projectionMatrix);

			for (int i = 0; i < 3; i++) {
				cubeColor[i] = rand() % 1000 / 1000.0f;
			}
			basicShader.SetUniform4f("u_color", cubeColor[0], cubeColor[1], cubeColor[2], cubeColor[3]);

			renderer.Draw(cubeVertexArray, cubeIndexBuffer, basicShader);
		}

		glfwSwapBuffers(window);

		/* Handle input controls --------------------------*/

		if (INPUT_STATE >= SHIFT_KEY) {
			cameraSpeed = 4.0f;
		}
		else {
			cameraSpeed = 2.0f;
		}

		if (!(INPUT_STATE >= W_KEY + S_KEY)) {
			if (INPUT_STATE >= W_KEY) {
				cameraPos += cameraSpeed * cameraDirection * deltaTime;
			}
			else if (INPUT_STATE >= S_KEY) {
				cameraPos -= cameraSpeed * cameraDirection * deltaTime;
			}
		}
		if (!(INPUT_STATE >= A_KEY + D_KEY)) {
			if (INPUT_STATE >= A_KEY) {
				cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed * deltaTime;
			}
			else if (INPUT_STATE >= D_KEY) {
				cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed * deltaTime;
			}
		}

		/* Mouse updates ----------------------------------------------*/

		pitch -= MOUSE_Y_DIFFERENCE * mouseSensitivity;
		yaw += MOUSE_X_DIFFERENCE * mouseSensitivity;

		MOUSE_X_DIFFERENCE = 0.0;
		MOUSE_Y_DIFFERENCE = 0.0;

	    /*if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}*/

		if (INPUT_STATE >= ESC_KEY) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}