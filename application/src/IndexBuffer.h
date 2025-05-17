#pragma once

class IndexBuffer {
private:
    unsigned int m_BufferID;

public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind();
    void Unbind();
};
