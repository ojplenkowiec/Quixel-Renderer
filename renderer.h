#ifndef RENDERER_H
#define RENDERER_H

#include "debugging.h"

#include <glfw3.h>

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "camera.h"
#include "window.h"

class Renderer // Debate over static or singleton?
{
private:
    Window* m_TargetWindowPtr;

public:
    Camera m_Camera;

    Renderer(Window* targetWindow, int vSync);

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
    void Draw(const VertexArray& va, Shader& shader);
    void DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader);
    void DrawLines(const VertexArray& va, Shader& shader);
};

#endif