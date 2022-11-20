#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "debugging.h"

#include "globals.h"
#include "inputstate.h"

#include <iostream>

static void glfwWindowCloseCallback(GLFWwindow* window) {
	std::cout << "Event: Window closed" << std::endl;
}

static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	switch (action) {
		case GLFW_PRESS:
			switch (button) {
				case GLFW_MOUSE_BUTTON_LEFT:
					INPUT_STATE += LEFT_MOUSE_BUTTON;
					break;
				case GLFW_MOUSE_BUTTON_RIGHT:
					INPUT_STATE += RIGHT_MOUSE_BUTTON;
					break;
			}
			break;
		case GLFW_RELEASE:
			switch (button) {
				case GLFW_MOUSE_BUTTON_LEFT:
					INPUT_STATE -= LEFT_MOUSE_BUTTON;
					break;
				case GLFW_MOUSE_BUTTON_RIGHT:
					INPUT_STATE -= RIGHT_MOUSE_BUTTON;
					break;
			}
			break;
	}
}

static void glfwMousePositionCallback(GLFWwindow* window, double xPos, double yPos) {
	g_MOUSE_X_DIFFERENCE = xPos - g_MOUSE_X;
	g_MOUSE_Y_DIFFERENCE = yPos - g_MOUSE_Y;

	g_MOUSE_X = xPos;
	g_MOUSE_Y = yPos;

	if (g_firstMouse) {
		g_MOUSE_X_DIFFERENCE = 0.0;
		g_MOUSE_Y_DIFFERENCE = 0.0;
		g_firstMouse = false;
	}
}

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action) {
	case GLFW_PRESS:
		switch (key) {
		case GLFW_KEY_W:
			INPUT_STATE += W_KEY;
			break;
		case GLFW_KEY_A:
			INPUT_STATE += A_KEY;
			break;
		case GLFW_KEY_S:
			INPUT_STATE += S_KEY;
			break;
		case GLFW_KEY_D:
			INPUT_STATE += D_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_Q:
			INPUT_STATE += Q_KEY;
			break;
		case GLFW_KEY_E:
			INPUT_STATE += E_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_SPACE:
			INPUT_STATE += SPACE_KEY;
			break;
		case GLFW_KEY_ENTER:
			INPUT_STATE += ENTER_KEY;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			INPUT_STATE += SHIFT_KEY;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			INPUT_STATE += CTRL_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_UP:
			INPUT_STATE += UP_KEY;
			break;
		case GLFW_KEY_RIGHT:
			INPUT_STATE += RIGHT_KEY;
			break;
		case GLFW_KEY_DOWN:
			INPUT_STATE += DOWN_KEY;
			break;
		case GLFW_KEY_LEFT:
			INPUT_STATE += LEFT_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_BACKSPACE:
			INPUT_STATE += BACKSPACE_KEY;
			break;
		case GLFW_KEY_ESCAPE:
			INPUT_STATE += ESC_KEY;
			break;
		}
		break;
/*------------------------------------------*/
/*||||||||||||||||||||||||||||||||||||||||||*/
/*------------------------------------------*/
	case GLFW_RELEASE:
		switch (key) {
		case GLFW_KEY_W:
			INPUT_STATE -= W_KEY;
			break;
		case GLFW_KEY_A:
			INPUT_STATE -= A_KEY;
			break;
		case GLFW_KEY_S:
			INPUT_STATE -= S_KEY;
			break;
		case GLFW_KEY_D:
			INPUT_STATE -= D_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_Q:
			INPUT_STATE -= Q_KEY;
			break;
		case GLFW_KEY_E:
			INPUT_STATE -= E_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_SPACE:
			INPUT_STATE -= SPACE_KEY;
			break;
		case GLFW_KEY_ENTER:
			INPUT_STATE -= ENTER_KEY;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			INPUT_STATE -= SHIFT_KEY;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			INPUT_STATE -= CTRL_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_UP:
			INPUT_STATE -= UP_KEY;
			break;
		case GLFW_KEY_RIGHT:
			INPUT_STATE -= RIGHT_KEY;
			break;
		case GLFW_KEY_DOWN:
			INPUT_STATE -= DOWN_KEY;
			break;
		case GLFW_KEY_LEFT:
			INPUT_STATE -= LEFT_KEY;
			break;
/*------------------------------------------*/
		case GLFW_KEY_BACKSPACE:
			INPUT_STATE -= BACKSPACE_KEY;
			break;
		case GLFW_KEY_ESCAPE:
			INPUT_STATE -= ESC_KEY;
			break;
		}
		break;
	}
}

#endif