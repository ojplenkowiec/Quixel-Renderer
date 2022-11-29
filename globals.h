#ifndef GLOBALS_H
#define GLOBALS_H

#include "inputstate.h"
#include <glfw3.h>

InputState INPUT_STATE = InputState::null_state;

double g_MOUSE_X = 0.0;
double g_MOUSE_Y = 0.0;

double g_MOUSE_X_DIFFERENCE = 0.0;
double g_MOUSE_Y_DIFFERENCE = 0.0;
bool g_firstMouse = true;

#endif