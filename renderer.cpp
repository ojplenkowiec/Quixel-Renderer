#include "renderer.h"

Renderer::Renderer(Window* targetWindow, int vSync) // shouldnt have a camera as a member! should render from POV of a specific camera, not have a camera...
    :m_TargetWindowPtr(targetWindow), m_Camera(Camera(targetWindow->GetWidth() / (float)targetWindow->GetHeight(), 45.0f, 0.1f, 100000.0f))
{
    glfwSwapInterval(vSync);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glClear(GL_DEPTH_BUFFER_BIT); // clears depth buffer for if enabled
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_Camera.GetViewMatrix());
    shader.SetUniformMat4f("u_projection", m_Camera.GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // type could be unsigned short to optimise! make dynamic?
}

void Renderer::Draw(const VertexArray& va, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_Camera.GetViewMatrix());
    shader.SetUniformMat4f("u_projection", m_Camera.GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    shader.SetUniform3f("u_viewPos", m_Camera.GetPosition());

    va.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, va.GetVertexCount())); // type could be unsigned short to optimise! make dynamic?
}

void Renderer::DrawInstancedArrays(const VertexArray& instanceVertexArray, Shader& instanceShader)
{
    instanceShader.Bind();
    instanceShader.SetUniformMat4f("u_view", m_Camera.GetViewMatrix());
    instanceShader.SetUniformMat4f("u_projection", m_Camera.GetProjectionMatrix());

    instanceShader.SetUniform3f("u_viewPos", m_Camera.GetPosition());

    instanceVertexArray.Bind();

    glDrawArraysInstanced(GL_TRIANGLES, 0, instanceVertexArray.GetVertexCount(), instanceVertexArray.GetInstanceCount());
}

void Renderer::DrawLines(const VertexArray& va, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", m_Camera.GetViewMatrix());
    shader.SetUniformMat4f("u_projection", m_Camera.GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    va.Bind();
    glDrawArrays(GL_LINES, 0, va.GetVertexCount());
}
