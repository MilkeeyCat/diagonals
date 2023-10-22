#pragma once

class IndexBuffer {
    private:
        unsigned int rendererId;
        unsigned int count;

    public:
        //0x88E4 is GL_STATIC_DRAW xd
        IndexBuffer(const unsigned int *data, unsigned int count, unsigned int usage = 0x88E4);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        unsigned int GetCount() const { return count; }
        void UpdateData(const unsigned int *data, unsigned int size) const;
};
