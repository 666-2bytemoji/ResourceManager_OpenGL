#include "FontImage.h"
#include "FontResource.h"
#include "RenderManager.h"

SDL_Color FontImage::_defaultFontColor = { 155, 155, 155, 155 };

FontImage::FontImage(std::string str, std::shared_ptr<FontResource> font, Color4 color)
: _fontResource(font)
, _texID(0)
{
    _fontColor =
    {
        static_cast<Uint8>(color._r * 255),
        static_cast<Uint8>(color._g * 255),
        static_cast<Uint8>(color._b * 255),
        static_cast<Uint8>(color._a * 255)
    };
    
    BuildImage(str);
}


FontImage::FontImage(std::string str, std::shared_ptr<FontResource> font)
: _fontResource(font)
, _texID(0)
{
    _fontColor = _defaultFontColor;
    BuildImage(str);
}


FontImage::FontImage(std::string str)
: _fontResource(FontResource::GetDefaultType())
, _texID(0)
{
    _fontColor = _defaultFontColor;
    BuildImage(str);
}


FontImage::~FontImage()
{
    Clear();
}


void FontImage::BuildImage(std::string str)
{
    //ラスタライズしたビットマップを取得
    _textSurface = TTF_RenderUTF8_Blended(_fontResource->GetFontType(), str.c_str(), _fontColor);
    
    if (_textSurface == nullptr)
        return;
    
    LoadFontImage(_textSurface);
}


void FontImage::LoadFontImage(SDL_Surface *surface)
{
    unsigned width = surface->w;
    unsigned height = surface->h;
    
    glGenTextures(1, &_texID);
    
    Uint8 colors = surface->format->BytesPerPixel;
    auto texture_format = GL_RGB;
    
    //useAlpha
    if (colors == 4)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }
    
    //画像データとテクスチャiDを結びつける
    glBindTexture(GL_TEXTURE_2D, _texID);
    
    //テクスチャの割り当て
    glTexImage2D(GL_TEXTURE_2D, 0, colors
                 , width, height
                 , 0, texture_format, GL_UNSIGNED_BYTE
                 , surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}


void FontImage::Clear()
{
    _fontResource = nullptr;
    SDL_FreeSurface(_textSurface);
    glDeleteTextures(1, &_texID);
}


void FontImage::SetDefaultFontColor(Color4 fontColor)
{
    _defaultFontColor =
    {
        static_cast<Uint8>(fontColor._r * 255),
        static_cast<Uint8>(fontColor._g * 255),
        static_cast<Uint8>(fontColor._b * 255),
        static_cast<Uint8>(fontColor._a * 255)
    };
}


void FontImage::SetFontColor(Color4 fontColor)
{
    _fontColor =
    {
        static_cast<Uint8>(fontColor._r * 255),
        static_cast<Uint8>(fontColor._g * 255),
        static_cast<Uint8>(fontColor._b * 255),
        static_cast<Uint8>(fontColor._a * 255)
    };
}
