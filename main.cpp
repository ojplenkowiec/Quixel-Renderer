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

	/* GLFW Callbacks --------------------------------------------*/

	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, glfwMousePositionCallback);

	/* Buffer Data ---------------------------------------------*/

	float cubeVertices[] = { // 24
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

	float longCubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // LONG AS FUCKKKK
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	std::vector<glm::vec3> cubePositions;
	for (int i = 0; i < 50000; i++) {
		float x = (rand() % 1000 - 500) / 10.0f;
		float y = (rand() % 1000 - 500) / 10.0f;
		float z = (rand() % 1000 - 500) / 10.0f;
		cubePositions.push_back(glm::vec3(x, y, z)); // translations
		cubePositions.push_back(glm::vec3((x + 50) / 100.0f, (y + 50) / 100.0f, (z + 50) / 100.0f)); // colors
	}

	/* Buffer Creation -------------------------------*/

	VertexBuffer cubeVB = VertexBuffer(cubeVertices, 24 * sizeof(float), 8);
	VertexBufferLayout cubeVBL = VertexBufferLayout();
	cubeVBL.Push<float>(3);
	IndexBuffer cubeIB = IndexBuffer(cubeIndexes, 36);

	VertexBuffer longCubeVB = VertexBuffer(longCubeVertices, 6 * 6 * 6 * sizeof(float), 6 * 6 * 6);
	VertexBufferLayout longCubeVBL = VertexBufferLayout();
	longCubeVBL.Push<float>(3);
	longCubeVBL.Push<float>(3);

	VertexBuffer cubeInstancesVB = VertexBuffer(&cubePositions[0], cubePositions.size() * sizeof(glm::vec3), cubePositions.size());
	VertexBufferLayout cubeInstancesVBL = VertexBufferLayout();
	cubeInstancesVBL.Push<float>(3);
	cubeInstancesVBL.Push<float>(3);

	/* VAO Creation ----------------------------------*/

	VertexArray cubeVAO = VertexArray(cubeVB, cubeVBL);
	VertexArray longCubeVAO = VertexArray(longCubeVB, longCubeVBL);

	VertexArray instancedLongCubeVAO = VertexArray(longCubeVB, longCubeVBL, cubeInstancesVB, cubeInstancesVBL);

	/* Shader Creation --------------------------*/

	Shader basicShader = Shader("basic.shader");

	Shader instanceShader = Shader("instance.shader");

	/* Renderer instantiation --------------------------*/

	Renderer renderer = Renderer(1, mode->width / (float)mode->height);

	/* Camera vector data -------------------------------*/

	float mouseSensitivity = 0.1f;
	float cameraSpeed = 4.0f;
	float rotateSpeed = 90.0f;

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (glfwWindowShouldClose(window) != GLFW_TRUE) {
		renderer.Clear();
		updateDeltaTime(); // updates DELTA_TIME global

		instanceShader.SetUniformMat4f("u_model", glm::mat4(1.0f));
		//instanceShader.SetUniform3f("u_objectColor", 1.0, 1.0, 1.0);

		basicShader.SetUniformMat4f("u_model", glm::mat4(1.0f));
		basicShader.SetUniform3f("u_objectColor", 0.5, 0.5, 0.5);

		renderer.DrawInstancedArrays(instancedLongCubeVAO, instanceShader);

		// renderer.Draw(cubeVAO, cubeIB, basicShader);

		glfwSwapBuffers(window);

		/* Keyboard updates --------------------------*/

		if (INPUT_STATE >= SPACE_KEY) {
			cameraSpeed = 40.0f;
		}
		else {
			cameraSpeed = 20.0f;
		}

		if (!(INPUT_STATE >= SHIFT_KEY + CTRL_KEY)) {
			if (INPUT_STATE >= SHIFT_KEY) {
				renderer.r_camera.TranslateUp(cameraSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= CTRL_KEY) {
				renderer.r_camera.TranslateUp(-cameraSpeed * g_DELTA_TIME);
			}
		}

		if (!(INPUT_STATE >= W_KEY + S_KEY)) {
			if (INPUT_STATE >= W_KEY) {
				renderer.r_camera.MoveForwards(cameraSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= S_KEY) {
				renderer.r_camera.MoveForwards(-cameraSpeed * g_DELTA_TIME);
			}
		}
		if (!(INPUT_STATE >= A_KEY + D_KEY)) {
			if (INPUT_STATE >= A_KEY) {
				renderer.r_camera.MoveRight(-cameraSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= D_KEY) {
				renderer.r_camera.MoveRight(cameraSpeed * g_DELTA_TIME);
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