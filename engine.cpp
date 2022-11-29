#include "engine.h"

Engine::Engine(uint32_t width, uint32_t height, const char* name)
{
    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("Couldn't initialize GLFW!");
    }
    glfwSwapInterval(1); // vsync enabled by default

    m_TimeOfLastBufferSwap = 0.0f;
    m_TimeSinceLastBufferSwap = glfwGetTime();

    m_Window = new Window(width, height, name, false, true);
    m_Window->MakeCurrentOpenGLContext();

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Couldn't initialize GLEW!");
    }

    m_Camera = new Camera((float)width / (float)height, 90.0f, 0.1f, 10000.0f); // change to camera2d

    // DO CALLBACKS STUFF, CREATE GLOBALS CLASS IN ENGINE

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Engine::~Engine()
{
    delete m_Window;
    delete m_Camera;
}

void Engine::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glClear(GL_DEPTH_BUFFER_BIT); // clears depth buffer for if enabled
}

void Engine::Update()
{
    m_Window->SwapBuffers();

    m_TimeSinceLastBufferSwap = glfwGetTime() - m_TimeOfLastBufferSwap;
    m_TimeOfLastBufferSwap = glfwGetTime();

    glfwPollEvents(); // poll events eg. input n' shite
}

void Engine::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Engine::SetClearColorHex(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    glClearColor((float)r / (float)255, (float)g / (float)255, (float)b / (float)255, (float)a / (float)255);
}

bool Engine::ShouldClose()
{
    return (m_Window->ShouldClose());
}

void Engine::SetSync(uint32_t size)
{
    glfwSwapInterval(size);
}

void Engine::Terminate()
{
    m_Window->Destroy();
    glfwTerminate();
}

void Engine::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
    shader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // type could be unsigned short to optimise! make dynamic?
}
void Engine::Draw(const VertexArray& va, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
    shader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    shader.SetUniform3f("u_viewPos", m_Camera->GetPosition());

    va.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVertexCount())); // type could be unsigned short to optimise! make dynamic?
}
void Engine::DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader)
{
    instanceShader.Bind();
    instanceShader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
    instanceShader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix());

    instanceShader.SetUniform3f("u_viewPos", m_Camera->GetPosition());

    instanceVertexArray.Bind();

    glDrawArraysInstanced(GL_TRIANGLES, 0, instanceVertexArray.GetVertexCount(), instanceVertexArray.GetInstanceCount());
}
void Engine::DrawLines(const VertexArray& va, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
    shader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    va.Bind();
    glDrawArrays(GL_LINES, 0, va.GetVertexCount());
}