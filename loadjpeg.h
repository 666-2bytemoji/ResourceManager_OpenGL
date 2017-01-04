#ifndef loadjpeg_h
#define loadjpeg_h

#include <stdio.h>
#include <jpeglib.h>
#include <OpenGL/gl.h>
#include <vector>
#include <string>


class JPEG
{
public:
    static bool Load(std::vector<unsigned char>& out, unsigned &sizeX, unsigned &sizeY, std::string filename);
};

#endif /* loadjpeg_h */
