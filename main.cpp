#include "quixel.h"

int main() {
	Quixel engine = Quixel(600, 600, "Quixel Demo");
	engine.SetClearColor(RGB(0.55f, 0.85f, 1.0f));
	while (!engine.ShouldClose()) { // main loop
		engine.FillRect(200.0f, 200.0f, 200.0f, 200.0f, HexRGB(255, 183, 197));
		engine.Update();
	}
	engine.Terminate();
}