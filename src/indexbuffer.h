#include "glad/glad.h"

class IndexBuffer
{
public:
    IndexBuffer(unsigned int* indices, size_t size);
    ~IndexBuffer();

    void bind();

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
};