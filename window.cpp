#include "window.h"

Window::Window(unsigned int windowWidth, unsigned int windowHeight, const char* windowName, bool resizable, bool floating)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_FLOATING, floating);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_WindowID = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
	m_Width = windowWidth;
	m_Height = windowHeight;
}

Window::~Window()
{
	glfwDestroyWindow(m_WindowID);
}

void Window::MakeCurrentOpenGLContext()
{
	glfwMakeContextCurrent(m_WindowID);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_WindowID);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_WindowID);
}

void Window::RequestClose()
{
	glfwSetWindowShouldClose(m_WindowID, true);
}
