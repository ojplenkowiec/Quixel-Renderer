#ifndef RENDERER
#define RENDERER

#include "debugging.h"

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

class Renderer // Debate over static or singleton?
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

#endif