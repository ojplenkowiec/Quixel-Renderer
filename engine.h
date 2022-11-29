#ifndef RENDERER_H
#define RENDERER_H

#include "debugging.h"

#include <glfw3.h>

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "camera.h"
#include "window.h"

class Engine // Debate over static or singleton?
{
private:
    Window* m_Window;
    Camera* m_Camera;
    float m_TimeSinceLastBufferSwap;
    float m_TimeOfLastBufferSwap;

public:
    Engine(uint32_t width, uint32_t height, const char* name);
    ~Engine();

    void Clear() const;
    void Update();

    void SetClearColor(float r, float g, float b, float a);
    void SetClearColorHex(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
    
    bool ShouldClose();

    void SetSync(uint32_t size);

    inline float DeltaTime() { return m_TimeSinceLastBufferSwap; }

    ////////////////////////////////////////////////////////////////////////
    void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
    void Draw(const VertexArray& va, Shader& shader);
    void DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader);
    void DrawLines(const VertexArray& va, Shader& shader);

    void Terminate();
};

#endif