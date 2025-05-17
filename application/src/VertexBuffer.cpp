// third-party
#include "glad/glad.h"

// project includes
#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_BufferID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_BufferID)); }

void VertexBuffer::Bind() { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID)); }
void VertexBuffer::Unbind() { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
