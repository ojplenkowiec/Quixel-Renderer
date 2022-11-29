#ifndef QUIXEL2D_H
#define QUIXEL2D_H

#include "debugging.h"

#include <glfw3.h>

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"
#include "camera2d.h"
#include "window.h"

class Quixel2D // Debate over static or singleton?
{
private:
    Window* m_Window;
    Camera2D* m_Camera;
    float m_TimeSinceLastBufferSwap;
    float m_TimeOfLastBufferSwap;
    void Clear() const;
    Shader* m_TexShader;
    Shader* m_BlockShader;
public:
    Quixel2D(uint32_t width, uint32_t height, const char* name);
    ~Quixel2D();
    void Update();
    void SetClearColor(float r, float g, float b, float a);
    void SetClearColorHex(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
    bool ShouldClose();
    void SetSync(uint32_t size);
    inline float DeltaTime() { return m_TimeSinceLastBufferSwap; }
    void Terminate();

    void FillRect(float x, float y, float width, float height, glm::vec4 color);

    void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
    void Draw(const VertexArray& va, Shader& shader);
    void DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader);
    void DrawLines(const VertexArray& va, Shader& shader);
};

#endif