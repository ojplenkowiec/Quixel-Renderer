#ifndef RENDERER
#define RENDERER

#include "debugging.h"

#include <glfw3.h>

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "camera.h"

class Renderer // Debate over static or singleton?
{
public:
    Camera r_camera;

    float r_aspectRatio;

    Renderer(int vSync, float aspectRatio);
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
    void Draw(const VertexArray& va, Shader& shader);
    void DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader);
};

#endif