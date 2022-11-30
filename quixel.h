#ifndef QUIXEL2D_H
#define QUIXEL2D_H

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "debugging.h"

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "camera.h"
#include "window.h"
#include "color.h"

class Quixel // Debate over static or singleton?
{
private:
    Window* m_Window;
    Camera* m_Camera;
    float m_TimeSinceLastBufferSwap;
    float m_TimeOfLastBufferSwap;
    void Clear() const;
    Shader* m_TexShader;
    Shader* m_BlockShader;
public:
    Quixel(uint32_t width, uint32_t height, const char* name);
    ~Quixel();
    void Update();

    void SetClearColor(Color clearColor);

    bool ShouldClose();
    void SetSync(uint32_t size);
    inline float DeltaTime() { return m_TimeSinceLastBufferSwap; }
    void Terminate();

    void FillRect(float x, float y, float width, float height, Color fillColor);
    void DrawPoint(float x, float y, Color pointColor);
};

#endif