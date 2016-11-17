#ifndef FontImage_h
#define FontImage_h

#include "Color.h"
#include "FontResource.h"
#include <OpenGL/GL.h>
#include <SDL2/SDL.h>

class FontImage
{
public:
    FontImage(std::string str, std::shared_ptr<FontResource> fontType, Color4 color);
    FontImage(std::string str, std::shared_ptr<FontResource> fontType);
    FontImage(std::string str);
    ~FontImage();
    
    SDL_Surface* GetSurface() { return _textSurface; }
    GLuint GetTexID() { return _texID; }
    
    void BuildImage(std::string str);
    void SetFontType(std::shared_ptr<FontResource> fontType) { _fontResource = fontType; };
    void SetFontColor(Color4 fontColor);
    void Clear();
    
    static SDL_Color GetDefaultFontColor() { return _defaultFontColor; }
    static void SetDefaultFontColor(Color4 fontColor);
    
private:

    void LoadFontImage(SDL_Surface *surface);

    //文字色
    SDL_Color _fontColor;
    
    //SDLで読み込むテクスチャ
    SDL_Surface* _textSurface;

    GLuint _texID;
    
    //SDL_TTFで読み込んだ文字フォーマット
    std::shared_ptr<FontResource> _fontResource;
    
    static SDL_Color _defaultFontColor;
};

#endif /* FontImage_h */
