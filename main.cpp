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
#include "octree.h"
#include "boid.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

void ComputeBoids(Boid** boids, unsigned int numBoids, Octree* octree) {
	for (unsigned int i = 0; i < numBoids; i++) { // for each boid
		if (boids[i]->velocity.x || boids[i]->velocity.y || boids[i]->velocity.z) {
			boids[i]->velocity = (boids[i]->velocity / glm::length(boids[i]->velocity)) * 30.0f;
		}
		else {
			boids[i]->velocity = glm::vec3(rand() % 1000 - 500.0f, rand() % 1000 - 500.0f, rand() % 1000 - 500.0f);
			boids[i]->velocity = (boids[i]->velocity / glm::length(boids[i]->velocity)) * 30.0f;
		}
		std::vector<void*>* boidsInRange = new std::vector<void*>;
		octree->QueryCuboid(boids[i]->position - glm::vec3(boids[i]->viewRadius, boids[i]->viewRadius, boids[i]->viewRadius), boids[i]->position + glm::vec3(boids[i]->viewRadius, boids[i]->viewRadius, boids[i]->viewRadius), boidsInRange);

		unsigned int numberInRange = 0;
		glm::vec3 averageVelocities = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 averagePositions = glm::vec3(0.0f, 0.0f, 0.0f);

		if (boidsInRange->size() > 1) {
			for (int j = 0; j < boidsInRange->size(); j++) {
				if (static_cast<Boid*>(boidsInRange->at(j)) != boids[i]) {
					glm::vec3 jToI = static_cast<Boid*>(boidsInRange->at(j))->position - boids[i]->position;
					float distance = glm::length(jToI);
					if (distance < 10) {
						boids[i]->AddForce(-((jToI / distance) * (50.0f / distance)));
					}
					numberInRange++;

					averageVelocities += static_cast<Boid*>(boidsInRange->at(j))->velocity;
					averagePositions += static_cast<Boid*>(boidsInRange->at(j))->position;
				}
			}
			averageVelocities /= static_cast<float>(numberInRange);
			averagePositions /= static_cast<float>(numberInRange);

			boids[i]->AddForce(averageVelocities - boids[i]->velocity);
			boids[i]->AddForce(averagePositions - boids[i]->position);
		}
		delete(boidsInRange);
	}
}

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
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
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
		-2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f, // LONG AS FUCKKKK
		 2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f,  0.0f, -1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f,  0.0f, -1.0f,

		-2.0f, -2.0f,  2.0f,  0.0f,  0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  0.0f,  0.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  0.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  0.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f,  0.0f, 1.0f,

		-2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f,  2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f, -2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f, -2.0f,  2.0f, -1.0f,  0.0f,  0.0f,
		-2.0f,  2.0f,  2.0f, -1.0f,  0.0f,  0.0f,

		 2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f,  0.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f,  0.0f,  0.0f,

		-2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, -1.0f,  0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, -1.0f,  0.0f,

		-2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		 2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f,  1.0f,  0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f,  1.0f,  0.0f
	};

	const unsigned int numberOfBoids = 10000;
	Boid** boids = new Boid*[numberOfBoids]; // DELETE BOIDS AND BOIDS ARRAY
	for (unsigned int i = 0; i < numberOfBoids; i++) {
		float x = (rand() % 1000 - 500) / 1.0f;
		float y = (rand() % 1000 - 500) / 1.0f;
		float z = (rand() % 1000 - 500) / 1.0f;
		boids[i] = new Boid(glm::vec3(x, y, z), 15.0f);
	}

	glm::vec3* boidPositions = new glm::vec3[numberOfBoids]; // DELETE BOID POSITIONS ARRAY
	for (unsigned int i = 0; i < numberOfBoids; i++) {
		boidPositions[i] = boids[i]->position;
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

	VertexBuffer cubeInstancesVB = VertexBuffer(&boidPositions[0], numberOfBoids * sizeof(glm::vec3), numberOfBoids);
	VertexBufferLayout cubeInstancesVBL = VertexBufferLayout();
	cubeInstancesVBL.Push<float>(3);
	//cubeInstancesVBL.Push<float>(3);

	/* VAO Creation ----------------------------------*/

	VertexArray cubeVAO = VertexArray(cubeVB, cubeVBL);
	VertexArray longCubeVAO = VertexArray(longCubeVB, longCubeVBL);

	VertexArray instancedLongCubeVAO = VertexArray(longCubeVB, longCubeVBL, cubeInstancesVB, cubeInstancesVBL);

	/* Shader Creation --------------------------*/

	Shader normalShader = Shader("normal.shader");

	Shader basicShader = Shader("basic.shader");

	Shader instancedNormalShader = Shader("instanced_normal.shader");

	/* Renderer instantiation --------------------------*/

	Renderer renderer = Renderer(1, mode->width / (float)mode->height);

	/* Camera vector data -------------------------------*/

	float mouseSensitivity = 0.1f;
	float cameraSpeed{};
	float rotateSpeed = 90.0f;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	while (glfwWindowShouldClose(window) != GLFW_TRUE) {
		Octree* octree = new Octree(glm::vec3(-10000.0f, -10000.0f, -10000.0f), glm::vec3(10000.0f, 10000.0f, 10000.0f), 32);
		for (int i = 0; i < numberOfBoids; i++) {
			octree->PushData(&boids[i]->position, boids[i]);
		}

		std::vector<glm::vec3> octreeLinesData;
		octree->GetVertices(&octreeLinesData);

		/*std::vector<void*> queryResults;
		octree.QueryCuboid(glm::vec3(-100.0f, -100.0f, -100.0f), glm::vec3(100.0f, 100.0f, 100.0f), &queryResults);
		for (int i = 0; i < queryResults.size(); i++) {
			static_cast<Boid*>(queryResults[i])->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}*/
		ComputeBoids(boids, numberOfBoids, octree);

		VertexBuffer octreeVB = VertexBuffer(octreeLinesData.data(), octreeLinesData.size() * sizeof(glm::vec3), octreeLinesData.size());
		VertexBufferLayout octreeVBL = VertexBufferLayout();
		octreeVBL.Push<float>(3);
		VertexArray octreeVAO = VertexArray(octreeVB, octreeVBL);

		for (unsigned int i = 0; i < numberOfBoids; i++) {
			boids[i]->Move(g_DELTA_TIME);
			boidPositions[i] = boids[i]->position;
		}
		cubeInstancesVB.SubData(&boidPositions[0], numberOfBoids * sizeof(glm::vec3));

		renderer.Clear();

		updateDeltaTime();

		renderer.DrawInstancedArrays(instancedLongCubeVAO, instancedNormalShader);
		renderer.DrawLines(octreeVAO, basicShader);

		glfwSwapBuffers(window);


		/* Input --------------------------*/

		if (INPUT_STATE >= SPACE_KEY) {
			cameraSpeed = 2000.0f;
		}
		else {
			cameraSpeed = 500.0f;
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
		delete octree;
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	for (int i = 0; i < numberOfBoids; i++) {
		delete boids[i];
	}
	delete[] boids;
	delete[] boidPositions;
}
