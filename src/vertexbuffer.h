#include "glad/glad.h"

class VertexBuffer
{
public:
    VertexBuffer(float* vertices, size_t size);
    ~VertexBuffer();

    void bind();

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
};