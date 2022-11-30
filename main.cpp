#include "quixel.h"

int main() {
	int width = 1000;
	int height = 1000;
	Quixel engine = Quixel(width, height, "Quixel Demo");
	engine.SetClearColor(RGB(0.2f));
	engine.SetVSync(0);
	while (!engine.ShouldClose()) { // main loop
		engine.FillRect(width / 2.0f - 100.0f, height / 2.0f - 100.0f, 200.0f, 200.0f, HexRGB(255, 183, 197));
		engine.FillRect(width / 2.0f - 200.0f, height / 2.0f - 200.0f, 200.0f, 200.0f, HexRGB(197, 255, 183));
		engine.FillRect(width / 2.0f, height / 2.0f, 200.0f, 200.0f, HexRGB(183, 197, 255));
		engine.Update();
		std::cout << engine.GetFPS() << "fps\n";
	}
	engine.Terminate();
}