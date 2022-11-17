#ifndef DEBUGGING_TOOLS
    #define DEBUGGING_TOOLS

    #ifndef GLEW_STATIC
        #define GLEW_STATIC
        #include <glew.h>
    #endif

    #include <fstream>
    #include <iostream>

    #ifdef _DEBUG
        #define ASSERT(x) if (!(x)) __debugbreak();
        
        static void writeLog(const char* msg) {
            std::ofstream file;
            file.open("log.txt", std::ofstream::app | std::ofstream::out);
            file << msg << std::endl;
            file.close();
        }

        static void glfwErrorCallback(int error, const char* description) {
            std::cout << "Error: " << error << std::endl << description << std::endl;
            writeLog(description);
            throw std::runtime_error("GLFW Err.");
        }

        static void GLClearError()
        {
            while (glGetError() != GL_NO_ERROR);
        }

        static bool GLLogCall(const char* function, const char* file, int line)
        {
            while (GLenum error = glGetError()) {
                std::cout << "[OpenGL Error] (" << error << ") : " << function << " " << file << " : " << line << std::endl;
                return false;
            }
            return true;
        }

        #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__)) 
    #else
        #define ASSERT(x) if (!(x)) __debugbreak();

        static void writeLog(const char* msg) { }

        static void glfwErrorCallback(int error, const char* description) { }

        static void GLClearError() { }

        static bool GLLogCall(const char* function, const char* file, int line) { return true; }

        #define GLCall(x) x
    #endif
#endif