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

    double m_TimeSinceLastBufferSwap;
    double m_TimeOfLastBufferSwap;

    Shader* m_TexShader;
    Shader* m_InstancedRGBAQuadShader;

    VertexBuffer* m_BLQuadVB;
    VertexBufferLayout* m_BLQuadVBL;

    VertexArray* m_BLQuadInstanceVA;
    IndexBuffer* m_BLQuadIB;

    VertexBufferLayout* m_BLQuadInstanceDataVBL;

    std::vector<glm::vec4>* m_QuadInstancesData;

    void Clear() const;
    void UpdateQuadInstanceArray();
    void BindInstancedRGBAQuadShader();
public:
    Quixel(uint32_t width, uint32_t height, const char* name);
    ~Quixel();
    void Update();

    void SetClearColor(Color clearColor);
    void SetClearColor(Color* clearColor);

    bool ShouldClose();

    void SetVSync(uint32_t size);

    inline double GetDeltaTime() { return m_TimeSinceLastBufferSwap; }
    inline int GetFPS() { return (int)(1.0f / (float)m_TimeSinceLastBufferSwap); }

    void Terminate();

    void FillRect(float x, float y, float width, float height, Color fillColor);
};

#endif