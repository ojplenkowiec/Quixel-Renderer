#include "quixel.h"

int main() {
	int width = 1000;
	int height = 1000;
	Quixel engine = Quixel(width, height, "Quixel Demo");
	engine.SetClearColor(fRGB(0.2f));
	engine.SetVSync(0);
	while (!engine.ShouldClose()) { // main loop
		for (int i = 0; i < 10000; i++) {
			//engine.FillRect(rand() % width, rand() % height, rand() % 30, rand() % 30, iRGB(rand() % 255, rand() % 255, rand() % 255));
			engine.FillRect(width / 2.0f * i / 5000.0f, height / 2.0f * i / 5000.0f, 1, 1, iRGB(190, 220, 255));
		}
		engine.Update();
		std::cout << engine.GetFPS() << "fps\n";
	}
	engine.Terminate();
}