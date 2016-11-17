#include "ImageResource.h"
#include <assert.h>

ImageResource::ImageResource(std::string fileName)
    : Resource(fileName)
{
    static int totalID = 1;
    //使用可能なテクスチャIDかどうかを問い合わせる
    //glGenTextures(希望ID, 格納先);
    glGenTextures(1, &_id);
    totalID++;

    //sizeBuffers
    unsigned width, height;
    
    //decode
    std::vector<unsigned char> imageBuffer;
    unsigned error = lodepng::decode(imageBuffer, width, height, _fileName.c_str());
    _width = width;
    _height = height;
    
    assert(error == 0);
    
    //どのように画像がメモリに格納されているかを宣言する
    //第一引数 : メモリ上のアドレス境界の指定
    //第二引数 : 何バイトおきにデータが格納されているか
    //  4 : RGBA    1: RGB
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    //画像データとテクスチャiDを結びつける
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA
                 , _width, _height
                 , 0, GL_RGBA, GL_UNSIGNED_BYTE
                 , &imageBuffer[0]);
    
    //テクスチャを拡大・縮小する方法の指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    //テクスチャのST方向へのリピート指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

ImageResource::~ImageResource()
{
    glDeleteTextures(1, &_id);
}