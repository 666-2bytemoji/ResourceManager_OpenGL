  #include "ImageResource.h"
#include "lodepng.h"
#include "loadbmp.h"
#include "loadjpeg.h"
#include <assert.h>

ImageResource::ImageResource(std::string fileName)
    : Resource(fileName)
    , _format(ImageFormat::NONE)
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
    auto it = fileName.find(".bmp");
    if (it != std::string::npos)
        _format = ImageFormat::BMP;

    it = fileName.find(".jpg");
    if (it != std::string::npos)
        _format = ImageFormat::JPEG;
    
    it = fileName.find(".png");
    if (it != std::string::npos)
        _format = ImageFormat::PNG;
        
    bool rgba = false;
    switch (_format)
    {
        case ImageFormat::PNG:
            rgba = true;
            assert(lodepng::decode(imageBuffer, width, height, _fileName) == 0);
            break;
            
        case ImageFormat::BMP:
            assert(BMP::Load(imageBuffer, width, height, _fileName) == true);
            break;

        case ImageFormat::JPEG:
            assert(JPEG::Load(imageBuffer, width, height, _fileName) == true);
            break;
            
        default:
            return;
    }
    
    _width = width;
    _height = height;
    
    //どのように画像がメモリに格納されているかを宣言する
    //第一引数 : メモリ上のアドレス境界の指定
    //第二引数 : 何バイトおきにデータが格納されているか
    //  4 : RGBA    1: RGB
    if (rgba)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    else
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    //画像データとテクスチャiDを結びつける
    glBindTexture(GL_TEXTURE_2D, _id);
    
    if (rgba)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA
                 , _width, _height
                 , 0, GL_RGBA, GL_UNSIGNED_BYTE
                 , &imageBuffer[0]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB
                     , _width, _height
                     , 0, GL_RGB, GL_UNSIGNED_BYTE
                     , &imageBuffer[0]);
    
    //テクスチャを拡大・縮小する方法の指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    //テクスチャのST方向へのリピート指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}


ImageResource::~ImageResource()
{
    glDeleteTextures(1, &_id);
}