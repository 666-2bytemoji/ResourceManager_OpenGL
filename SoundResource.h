#ifndef SoundResource_h
#define SoundResource_h

#include "Resource.h"
#include <OpenAL/OpenAL.h>
#include <vorbis/vorbisfile.h>

#include <string>
#include <vector>

class SoundResource : public Resource
{
public:
    SoundResource(std::string fileName);
    ~SoundResource();
    
    ALuint GetBufferID() const { return _buffer; };
    
private:

    void EncodeAsWAV();
    void EncodeAsOgg();
    void Init();
    int LoadWAV(FILE* fp, int *channel, int* bit, int *size, int* freq);
    
    ALuint _buffer;
};

#endif /* SoundResource_h */
