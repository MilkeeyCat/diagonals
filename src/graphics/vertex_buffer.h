#pragma once

class VertexBuffer {
    private:
        unsigned int rendererId;
    public:
        //0x88E4 is GL_STATIC_DRAW xd
        VertexBuffer(const void *data, unsigned int size, unsigned int usage = 0x88E4);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
        void UpdateData(const void *data, unsigned int size) const;
};
