// third-party
#include "glad/glad.h"

// project includes
#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_BufferID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
                        GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_BufferID); }

void IndexBuffer::Bind() { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID)); }
void IndexBuffer::Unbind() { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
