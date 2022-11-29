#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "debugging.h"

#include "callbacks.h"
#include "inputstate.h"
#include "window.h"
#include "quixel2d.h"
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
	Quixel2D engine = Quixel2D(1920, 1080, "Quixel Demo");
	engine.SetClearColorHex(0, 0, 0, 255);

	int i = 0;
	while (!engine.ShouldClose()) { // main loop
		float r = (i % 255) / 255.0f;
		float g = ((i + 255 / 3) % 255) / 255.0f;
		float b = (((i + 255 / 3) * 2) % 255) / 255.0f;

		for (int j = 0; j < 100; j++) {
			engine.FillRect(0.0f, 0.0f, 200.0f, 267.0f, glm::vec4(r, g, b, 1.0f));
		}
		engine.Update();
		i++;
	}

	engine.Terminate();
}
