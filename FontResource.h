#ifndef FontResource_h
#define FontResource_h

#include "Color.h"
#include "Resource.h"
#include <OpenGL/GL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class FontResource : public Resource
{
public:
    FontResource(std::string ttfFilePath, int fontSize = 16);
    ~FontResource();

    TTF_Font* GetFontType() { return _fontType; }
    int GetFontSize() const { return _fontSize; }

    void Clear();
    
    static void SetDefaultType(std::string ttfFilePath, int fontSize = 16);
    static std::shared_ptr<FontResource> GetDefaultType() { return _defaultFont; }
    
private:

    //文字サイズ
    int _fontSize;
    
    //SDL_TTFで読み込んだ文字フォーマット
    TTF_Font* _fontType;
    
    static std::shared_ptr<FontResource> _defaultFont;
};

#endif /* FontResource_h */
