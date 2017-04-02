#ifndef ShaderResource_h
#define ShaderResource_h

#include "Resource.h"
#include <OpenGL/GL.h>

class ShaderResource : public Resource
{
public:
    ShaderResource(std::string shaderFilePath);
    ~ShaderResource();
    
    GLuint GetID() const { return _id; }
    void PrintLog();
    
private:

    void Load(std::string shaderFilePath);
    GLuint _id;
};

#endif /* ShaderResource_h */
