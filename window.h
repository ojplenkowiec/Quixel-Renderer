#ifndef WINDOW_H
#define WINDOW_H

#include <glew.h>
#include <glfw3.h>
#include "debugging.h"

class Window {
private:
	GLFWwindow* m_WindowID;
	unsigned int m_Width;
	unsigned int m_Height;
	bool m_Resizable;
public:	
	Window(unsigned int windowWidth, unsigned int windowHeight, const char* windowName, bool resizable, bool floating);
	~Window();
	void MakeCurrentOpenGLContext();
	void SwapBuffers();
	bool ShouldClose();
	void RequestClose();
	void Destroy();

	inline GLFWwindow* GetID() const { return m_WindowID; }
	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }
};

#endif
