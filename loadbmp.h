#ifndef loadbmp_h
#define loadbmp_h

#include <OpenGL/gl.h>
#include <vector>
#include <string>

class BMP
{
public:
    static bool Load(std::vector<unsigned char>& out, unsigned &sizeX, unsigned &sizeY, std::string filename);
};

#endif /* loadbmp_h */
