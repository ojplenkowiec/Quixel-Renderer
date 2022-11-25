#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "debugging.h"

#include "callbacks.h"
#include "inputstate.h"
#include "globals.h"
#include "window.h"
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"
#include "octree.h"
#include "boid.h"
#include "randomranges.h"
#include "boidbehavior.h"
#include "prefabs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <random>

int main() {
	const unsigned int numberOfBoids = 10000;

	float mouseSensitivity = 0.15f;
	float cameraSpeed = 500.0f;
	float rotateSpeed = 90.0f;

	bool gridOn = false;

	if (glfwInit() != GLFW_TRUE) {
		throw std::runtime_error("Couldn't initialize GLFW!");
	}

	Window mainWindow = Window(1000, 1000, "Quixel Engine", false, true);
	mainWindow.MakeCurrentOpenGLContext();

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Couldn't initialize GLEW!");
	}

	/* GLFW Callbacks --------------------------------------------*/

	glfwSetErrorCallback(glfwErrorCallback);
	glfwSetWindowCloseCallback(mainWindow.GetID(), glfwWindowCloseCallback);
	glfwSetKeyCallback(mainWindow.GetID(), glfwKeyCallback);
	glfwSetMouseButtonCallback(mainWindow.GetID(), glfwMouseButtonCallback);
	glfwSetCursorPosCallback(mainWindow.GetID(), glfwMousePositionCallback); // add to window class

	Boid** boids = new Boid*[numberOfBoids];
	for (unsigned int i = 0; i < numberOfBoids; i++) {
		float x = RandomFloat(-10000, 10000);
		float y = RandomFloat(-10000, 10000);
		float z = RandomFloat(-10000, 10000);
		boids[i] = new Boid(glm::vec3(x / 50.0f, y / 50.0f, z / 50.0f), 30.0f);
	}

	glm::vec3* boidPositions = new glm::vec3[numberOfBoids * 2];
	for (unsigned int i = 0; i < numberOfBoids; i++) {
		boidPositions[2 * i] = boids[i]->position;
		boidPositions[2 * i + 1] = boids[i]->color;
	}

	/* Buffer Creation -------------------------------*/

	VertexBuffer cubeVB = VertexBuffer(indexedCubeVertices, 24 * sizeof(float), 8);
	VertexBufferLayout cubeVBL = VertexBufferLayout();
	cubeVBL.Push<float>(3);
	IndexBuffer cubeIB = IndexBuffer(cubeIndexes, 36);

	VertexBuffer longCubeVB = VertexBuffer(longCubeVertices, 6 * 6 * 6 * sizeof(float), 6 * 6 * 6);
	VertexBufferLayout longCubeVBL = VertexBufferLayout();
	longCubeVBL.Push<float>(3);
	longCubeVBL.Push<float>(3);

	VertexBuffer cubeInstancesVB = VertexBuffer(&boidPositions[0], numberOfBoids * sizeof(glm::vec3) * 2, numberOfBoids);
	VertexBufferLayout cubeInstancesVBL = VertexBufferLayout();
	cubeInstancesVBL.Push<float>(3);
	cubeInstancesVBL.Push<float>(3);

	/* VAO Creation ----------------------------------*/

	VertexArray cubeVAO = VertexArray(cubeVB, cubeVBL);
	VertexArray longCubeVAO = VertexArray(longCubeVB, longCubeVBL);
	VertexArray instancedLongCubeVAO = VertexArray(longCubeVB, longCubeVBL, cubeInstancesVB, cubeInstancesVBL);

	/* Shader Creation --------------------------*/

	Shader normalShader = Shader("normal.shader");
	Shader basicShader = Shader("basic.shader");
	Shader instancedNormalShader = Shader("instanced_normal.shader");

	/* Renderer instantiation --------------------------*/

	Renderer renderer = Renderer(&mainWindow, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	while (!mainWindow.ShouldClose()) {
		renderer.Clear();
		updateDeltaTime();

		Octree* octree = new Octree(glm::vec3(-10000.0f, -10000.0f, -10000.0f), glm::vec3(10000.0f, 10000.0f, 10000.0f), 16);
		for (int i = 0; i < numberOfBoids; i++) {
			octree->PushData(&boids[i]->position, boids[i]);
		}

		std::vector<glm::vec3> octreeLinesData;
		octree->GetVertices(&octreeLinesData);

		ComputeBoids(boids, numberOfBoids, octree);

		for (unsigned int i = 0; i < numberOfBoids; i++) {
			boidPositions[2 * i] = boids[i]->position; // for sub buffer data
			boidPositions[2 * i + 1] = boids[i]->color;

			if (boids[i]->color != glm::vec3(1.0, 1.0, 1.0)) { // slowly reset color to white
				boids[i]->color += glm::vec3(0.01, 0.01, 0.01);
				if (boids[i]->color.x > 1.0 && boids[i]->color.y > 1.0 && boids[i]->color.z > 1.0) {
					boids[i]->color = glm::vec3(1.0, 1.0, 1.0);
				}
			}
		}

		cubeInstancesVB.SubData(&boidPositions[0], numberOfBoids * sizeof(glm::vec3) * 2);

		renderer.DrawInstancedArrays(instancedLongCubeVAO, instancedNormalShader);

		if (gridOn) {
			VertexBuffer octreeVB = VertexBuffer(octreeLinesData.data(), octreeLinesData.size() * sizeof(glm::vec3), octreeLinesData.size());
			VertexBufferLayout octreeVBL = VertexBufferLayout();
			octreeVBL.Push<float>(3);
			VertexArray octreeVAO = VertexArray(octreeVB, octreeVBL);
			renderer.DrawLines(octreeVAO, basicShader);
		}

		mainWindow.SwapBuffers();

		/* Input --------------------------*/

		if (!(INPUT_STATE >= SHIFT_KEY + CTRL_KEY)) {
			if (INPUT_STATE >= SPACE_KEY) {
				if (INPUT_STATE >= SHIFT_KEY) {
					renderer.m_Camera.TranslateUp(cameraSpeed * 4.0f * g_DELTA_TIME);
				}
				else if (INPUT_STATE >= CTRL_KEY) {
					renderer.m_Camera.TranslateUp(-cameraSpeed * 4.0f * g_DELTA_TIME);
				}
			}
			else {
				if (INPUT_STATE >= SHIFT_KEY) {
					renderer.m_Camera.TranslateUp(cameraSpeed * g_DELTA_TIME);
				}
				else if (INPUT_STATE >= CTRL_KEY) {
					renderer.m_Camera.TranslateUp(-cameraSpeed * g_DELTA_TIME);
				}
			}
		}

		if (!(INPUT_STATE >= W_KEY + S_KEY)) {
			if (INPUT_STATE >= SPACE_KEY) {
				if (INPUT_STATE >= W_KEY) {
					renderer.m_Camera.MoveForwards(cameraSpeed * 4.0f * g_DELTA_TIME);
				}
				else if (INPUT_STATE >= S_KEY) {
					renderer.m_Camera.MoveForwards(-cameraSpeed * 4.0f * g_DELTA_TIME);
				}
			}
			else {
				if (INPUT_STATE >= W_KEY) {
					renderer.m_Camera.MoveForwards(cameraSpeed * g_DELTA_TIME);
				}
				else if (INPUT_STATE >= S_KEY) {
					renderer.m_Camera.MoveForwards(-cameraSpeed * g_DELTA_TIME);
				}
			}
		}

		if (!(INPUT_STATE >= A_KEY + D_KEY)) {
			if (INPUT_STATE >= SPACE_KEY) {
				if (INPUT_STATE >= A_KEY) {
					renderer.m_Camera.MoveRight(-cameraSpeed * 4.0f * g_DELTA_TIME);
				}
				else if (INPUT_STATE >= D_KEY) {
					renderer.m_Camera.MoveRight(cameraSpeed * 4.0f * g_DELTA_TIME);
				}
			}
			else {
				if (INPUT_STATE >= A_KEY) {
					renderer.m_Camera.MoveRight(-cameraSpeed * g_DELTA_TIME);
				}
				else if (INPUT_STATE >= D_KEY) {
					renderer.m_Camera.MoveRight(cameraSpeed * g_DELTA_TIME);
				}
			}
		}

		if (!(INPUT_STATE >= LEFT_KEY + RIGHT_KEY)) {
			if (INPUT_STATE >= LEFT_KEY) {
				renderer.m_Camera.Rotate(0.0f, -rotateSpeed * g_DELTA_TIME);
			}
			else if (INPUT_STATE >= RIGHT_KEY) {
				renderer.m_Camera.Rotate(0.0f, rotateSpeed * g_DELTA_TIME);
			}
		}

		if (!(INPUT_STATE >= UP_KEY + DOWN_KEY)) {
			if (INPUT_STATE >= UP_KEY) {
				renderer.m_Camera.Rotate(rotateSpeed * g_DELTA_TIME, 0.0f);
			}
			else if (INPUT_STATE >= DOWN_KEY) {
				renderer.m_Camera.Rotate(-rotateSpeed * g_DELTA_TIME, 0.0f);
			}
		}

		if (INPUT_STATE >= ESC_KEY) {
			mainWindow.RequestClose();
		}

		if (INPUT_STATE >= Q_KEY) { // flip flop for grid
			INPUT_STATE -= Q_KEY;
			gridOn = !gridOn;
		}

		if (INPUT_STATE >= ENTER_KEY) { // reset boids
			INPUT_STATE -= ENTER_KEY;
			for (int i = 0; i < numberOfBoids; i++) {
				boids[i]->position = glm::vec3(RandomFloat(-10000, 10000) / 50.0f, RandomFloat(-10000, 10000) / 50.0f, RandomFloat(-10000, 10000) / 50.0f);
				boids[i]->velocity = glm::vec3(0.0f);
			}
		}

		if (INPUT_STATE >= LEFT_MOUSE_BUTTON) {
			glm::vec3 rayWorld = renderer.m_Camera.GetRay(g_MOUSE_X, g_MOUSE_Y, mainWindow.GetWidth(), mainWindow.GetHeight()); // calculate ray

			if (INPUT_STATE >= E_KEY) {
				std::vector<void*> collisionsInRange = octree->RayCastRange(renderer.m_Camera.GetPosition(), rayWorld, 20.0f);
				for (int i = 0; i < collisionsInRange.size(); i++) {
					static_cast<Boid*>(collisionsInRange[i])->color = glm::vec3(0.0, 1.0, 0.0);
				}
			}
			else {
				Boid* closestCollision = static_cast<Boid*>(octree->RayCastClosest(renderer.m_Camera.GetPosition(), rayWorld, 3.0f));

				if (closestCollision != nullptr) {
					closestCollision->color = glm::vec3(1.0, 0.0, 0.0);
				}
			}
		}

		if (INPUT_STATE >= RIGHT_MOUSE_BUTTON) {
			renderer.m_Camera.Rotate(-g_MOUSE_Y_DIFFERENCE * mouseSensitivity, g_MOUSE_X_DIFFERENCE * mouseSensitivity);

			g_MOUSE_X_DIFFERENCE = 0.0;
			g_MOUSE_Y_DIFFERENCE = 0.0; // should be done in a class?
		}

		/* Poll events --------------------------------------*/

		glfwPollEvents();

		delete octree;
	}

	glfwTerminate();
	for (int i = 0; i < numberOfBoids; i++) {
		delete boids[i];
	}
	delete[] boids;
	delete[] boidPositions;
}
