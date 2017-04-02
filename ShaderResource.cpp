#include "ShaderResource.h"
#include <fstream>
#include <cassert>
#include <vector>
#include "../Resources/ResourceManager.h"

ShaderResource::ShaderResource(std::string shaderFilePath)
: Resource(shaderFilePath)
, _id(0)
{
    Load(shaderFilePath);
}


ShaderResource::~ShaderResource()
{
    if (_id != 0)
        glDeleteShader(_id);
}


void ShaderResource::Load(std::string shaderFilePath)
{
    std::ifstream fs(shaderFilePath);
    assert (fs.is_open());
    
    std::istreambuf_iterator<char> dataBegin(fs);
    std::istreambuf_iterator<char> dataEnd;
    std::string fileData(dataBegin, dataEnd);
    
    //shaderファイルを文字列として読み込む
    const GLchar *vsrc = fileData.c_str();
    
    //shaderをコンパイル
    glShaderSource(_id, 1, &vsrc, NULL);
    glCompileShader(_id);
}


void ShaderResource::PrintLog()
{
    GLsizei bufSize = 0;
    
    //シェーダのコンパイル時のログの長さを取得する
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH , &bufSize);
    
    if(bufSize > 1)
    {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        
        //シェーダのコンパイル時のログの内容を取得する
        glGetShaderInfoLog(_id, bufSize, &length, &infoLog[0]);
        printf("InfoLog:\n%s", &infoLog[0]);
    }
}
