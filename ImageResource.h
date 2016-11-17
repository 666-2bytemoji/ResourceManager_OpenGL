#ifndef ImageResource_h
#define ImageResource_h

#include "Resource.h"
#include "lodepng.h"
#include <vector>
#include <OpenGL/gl.h>

class ImageResource : public Resource
{
public:
    ImageResource(std::string fileName);    
    ~ImageResource();

    GLuint GetID() const { return _id; }
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    
private:

    GLuint _id;
    int _width;
    int _height;
};


#endif /* ImageResource_h */
