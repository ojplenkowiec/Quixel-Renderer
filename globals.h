#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

#include "inputstate.h"
#include <glfw3.h>

InputState INPUT_STATE = InputState::null_state;

double g_MOUSE_X = 0.0;
double g_MOUSE_Y = 0.0;

double g_MOUSE_X_DIFFERENCE = 0.0;
double g_MOUSE_Y_DIFFERENCE = 0.0;
bool g_firstMouse = true;

float g_DELTA_TIME = 0.0f;
float g_CURRENT_TIME = 0.0f;

static void updateDeltaTime() { // Should be called every frame!
	float glfwTime = glfwGetTime();
	g_DELTA_TIME = glfwTime - g_CURRENT_TIME;
	g_CURRENT_TIME = glfwTime;
}

#endif