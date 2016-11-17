#include "Resource.h"
#include "ResourceManager.h"

Resource::Resource(std::string fileName)
: _fileName(fileName)
//, _handle(NULL)
{
}

Resource::~Resource()
{
}

bool Resource::Load()
{
    return false;
}

bool Resource::Unload()
{
    return false;
}