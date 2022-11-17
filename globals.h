#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

#include "inputstate.h"

InputState INPUT_STATE = InputState::null_state;

double MOUSE_X = 0.0;
double MOUSE_Y = 0.0;

double MOUSE_X_DIFFERENCE = 0.0;
double MOUSE_Y_DIFFERENCE = 0.0;
bool firstMouse = true;

#endif