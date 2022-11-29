#include "quixel2d.h"

Quixel2D::Quixel2D(uint32_t width, uint32_t height, const char* name)
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

    m_Camera = new Camera2D(width, height);

    m_TexShader = new Shader("quixel2dtex.shader");
    m_BlockShader = new Shader("quixel2dblock.shader");

    // DO CALLBACKS STUFF, CREATE GLOBALS CLASS IN ENGINE

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Quixel2D::~Quixel2D()
{
    delete m_Window;
    delete m_Camera;
    delete m_TexShader;
    delete m_BlockShader;
}

void Quixel2D::Update()
{
    m_Window->SwapBuffers();

    m_TimeSinceLastBufferSwap = glfwGetTime() - m_TimeOfLastBufferSwap;
    m_TimeOfLastBufferSwap = glfwGetTime();

    glfwPollEvents(); // poll events eg. input n' shite
    Clear();
}
void Quixel2D::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glClear(GL_DEPTH_BUFFER_BIT); // clears depth buffer for if enabled
}

void Quixel2D::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Quixel2D::SetClearColorHex(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    glClearColor((float)r / (float)255, (float)g / (float)255, (float)b / (float)255, (float)a / (float)255);
}

bool Quixel2D::ShouldClose()
{
    return (m_Window->ShouldClose());
}

void Quixel2D::SetSync(uint32_t size)
{
    glfwSwapInterval(size);
}

void Quixel2D::Terminate()
{
    m_Window->Destroy();
    glfwTerminate();
}

void Quixel2D::FillRect(float x, float y, float width, float height, glm::vec4 color)
{
    m_BlockShader->Bind();
    m_BlockShader->SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
    m_BlockShader->SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix());
    // m_BlockShader->SetUniformMat4f("u_model", glm::scale(glm::mat4(1.0f), glm::vec3(x2 - x1, y2 - y1, 1.0f)));
    m_BlockShader->SetUniformMat4f("u_model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)), glm::vec3(width, height, 1.0f)));
    // create a model matrix from aabb points

    m_BlockShader->SetUniform4f("u_color", color);

    float vertices[] = {
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    unsigned int indexes[] = {
        0, 1, 2,
        3, 0, 2
    };

    VertexBuffer vb = VertexBuffer(&vertices[0], sizeof(float) * 3 * 4);
    VertexBufferLayout vbl = VertexBufferLayout();
    vbl.Push<float>(3);

    VertexArray va = VertexArray(vb, vbl);

    IndexBuffer ib = IndexBuffer(&indexes[0], 6);

    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

// void Engine::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
// {
//     shader.Bind();
//     shader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
//     shader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...
// 
//     va.Bind();
//     ib.Bind();
// 
//     GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // type could be unsigned short to optimise! make dynamic?
// }
// void Engine::Draw(const VertexArray& va, Shader& shader)
// {
//     shader.Bind();
//     shader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
//     shader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...
// 
//     shader.SetUniform3f("u_viewPos", m_Camera->GetPosition());
// 
//     va.Bind();
// 
//     GLCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVertexCount())); // type could be unsigned short to optimise! make dynamic?
// }
// void Engine::DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader)
// {
//     instanceShader.Bind();
//     instanceShader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
//     instanceShader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix());
// 
//     instanceShader.SetUniform3f("u_viewPos", m_Camera->GetPosition());
// 
//     instanceVertexArray.Bind();
// 
//     glDrawArraysInstanced(GL_TRIANGLES, 0, instanceVertexArray.GetVertexCount(), instanceVertexArray.GetInstanceCount());
// }
// void Engine::DrawLines(const VertexArray& va, Shader& shader)
// {
//     shader.Bind();
//     shader.SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
//     shader.SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...
// 
//     va.Bind();
//     glDrawArrays(GL_LINES, 0, va.GetVertexCount());
// }

// glfwSetErrorCallback(glfwErrorCallback);
    // glfwSetWindowCloseCallback(gameWindow.GetID(), glfwWindowCloseCallback);
    // glfwSetKeyCallback(gameWindow.GetID(), glfwKeyCallback);
    // glfwSetMouseButtonCallback(gameWindow.GetID(), glfwMouseButtonCallback);
    // glfwSetCursorPosCallback(gameWindow.GetID(), glfwMousePositionCallback);

    /* Buffer Creation -------------------------------*/

    /* VAO Creation ----------------------------------*/

    /* Shader Creation --------------------------*/

    /* Renderer instantiation --------------------------*/