#include "quixel.h"

Quixel::Quixel(uint32_t width, uint32_t height, const char* name)
{
    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("Couldn't initialize GLFW!");
    }

    m_Window = new Window(width, height, name, false, true);
    m_Window->MakeCurrentOpenGLContext();

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Couldn't initialize GLEW!");
    }

    m_Camera = new Camera(width, height);

    m_TexShader = new Shader("tex.shader");
    m_InstancedRGBAQuadShader = new Shader("InstancedAABBs.shader");

    glfwSetErrorCallback(glfwErrorCallback);
    glfwSwapInterval(1); // vsync enabled by default

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_QuadInstancesData = new std::vector<glm::vec4>();

    float BLQuadVertices[] = {
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    unsigned int QuadIndexes[] = {
        0, 1, 2,
        3, 0, 2
    };
    m_BLQuadVB = new VertexBuffer(&BLQuadVertices[0], sizeof(float) * 3 * 4);
    m_BLQuadVBL = new VertexBufferLayout();
    m_BLQuadVBL->Push<float>(3);

    m_BLQuadInstanceVA = new VertexArray(*m_BLQuadVB, *m_BLQuadVBL); // fix this deref
    m_BLQuadIB = new IndexBuffer(&QuadIndexes[0], 6);

    m_BLQuadInstanceDataVBL = new VertexBufferLayout();
    m_BLQuadInstanceDataVBL->Push<float>(4);
    m_BLQuadInstanceDataVBL->Push<float>(4);
    m_BLQuadInstanceDataVBL->Push<float>(4);
    m_BLQuadInstanceDataVBL->Push<float>(4);
    m_BLQuadInstanceDataVBL->Push<float>(4);

    m_TimeOfLastBufferSwap = 0.0f;
    m_TimeSinceLastBufferSwap = glfwGetTime();
    std::cout << m_TimeSinceLastBufferSwap * 1000.0f << "ms to initialize engine!\n";
}

Quixel::~Quixel()
{
    delete m_Window;
    delete m_Camera;
    delete m_TexShader;
    delete m_InstancedRGBAQuadShader;

    delete m_BLQuadVBL;
    delete m_BLQuadInstanceVA;
    delete m_BLQuadIB;
    delete m_QuadInstancesData;
    delete m_BLQuadInstanceDataVBL;
}

void Quixel::UpdateQuadInstanceArray()
{
    delete m_BLQuadInstanceVA; // delete
    VertexBuffer instanceDataVB = VertexBuffer(m_QuadInstancesData->data(), m_QuadInstancesData->size() * sizeof(float) * 4); // RGBA - ABCD,ABCD,ABCD,ABCD (MATRIX)

    m_BLQuadInstanceVA = new VertexArray(*m_BLQuadVB, *m_BLQuadVBL, instanceDataVB, *m_BLQuadInstanceDataVBL); // recreate, also fix need to deref here...
}

void Quixel::BindInstancedRGBAQuadShader()
{

}

void Quixel::Update()
{
    UpdateQuadInstanceArray(); // might not need doing every frame! add a check?

    m_InstancedRGBAQuadShader->Bind();
    m_InstancedRGBAQuadShader->SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
    m_InstancedRGBAQuadShader->SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix());

    m_BLQuadInstanceVA->Bind();
    m_BLQuadIB->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, m_QuadInstancesData->size() / 5);
    m_QuadInstancesData->clear(); // clear data for matrixes and colors

    m_Window->SwapBuffers();

    m_TimeSinceLastBufferSwap = glfwGetTime() - m_TimeOfLastBufferSwap;
    m_TimeOfLastBufferSwap = glfwGetTime();

    glfwPollEvents(); // poll events eg. input n' shite
    Clear();
}

void Quixel::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glClear(GL_DEPTH_BUFFER_BIT); // clears depth buffer for if enabled
}

void Quixel::SetClearColor(Color clearColor)
{
    glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
}

void Quixel::SetClearColor(Color* clearColor)
{
    glClearColor(clearColor->R(), clearColor->G(), clearColor->B(), clearColor->A());
}

bool Quixel::ShouldClose()
{
    return (m_Window->ShouldClose());
}

void Quixel::SetVSync(uint32_t size)
{
    glfwSwapInterval(size);
}

void Quixel::Terminate()
{
    m_Window->Destroy();
    glfwTerminate();
}

void Quixel::FillRect(float x, float y, float width, float height, Color fillColor)
{
    glm::mat4 modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)), glm::vec3(width, height, 1.0f));

    m_QuadInstancesData->push_back(fillColor.Data()); // push vec4 color
    m_QuadInstancesData->push_back(modelMatrix[0]);
    m_QuadInstancesData->push_back(modelMatrix[1]);
    m_QuadInstancesData->push_back(modelMatrix[2]);
    m_QuadInstancesData->push_back(modelMatrix[3]); // push mat4 model matrix as vec4s
}

// glfwSetWindowCloseCallback(gameWindow.GetID(), glfwWindowCloseCallback);
// glfwSetKeyCallback(gameWindow.GetID(), glfwKeyCallback);
// glfwSetMouseButtonCallback(gameWindow.GetID(), glfwMouseButtonCallback);
// glfwSetCursorPosCallback(gameWindow.GetID(), glfwMousePositionCallback);