#include "renderer.h"

Renderer::Renderer(int vSync, float aspectRatio)
    :r_aspectRatio(aspectRatio), r_camera(Camera(aspectRatio, 45.0f, 0.1f, 200.0f))
{
    glfwSwapInterval(vSync);
}

Renderer::~Renderer()
{
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glClear(GL_DEPTH_BUFFER_BIT); // clears depth buffer for if enabled
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
{
    shader.Bind();
    shader.SetUniformMat4f("u_view", r_camera.GetViewMatrix());
    shader.SetUniformMat4f("u_projection", r_camera.GetProjectionMatrix()); // maybe implement camera/no camera modes! also check to see if these uniforms exist, tell user to create them if they do not...

    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // type could be unsigned short to optimise! make dynamic?
}