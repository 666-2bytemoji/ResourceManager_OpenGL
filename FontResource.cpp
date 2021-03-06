#include "FontResource.h"
#include "ResourceManager.h"

std::shared_ptr<FontResource> FontResource::_defaultFont = nullptr;

FontResource::FontResource(std::string ttfFilePath, int fontSize)
: Resource(ttfFilePath)
, _fontType(nullptr)
, _fontSize(fontSize)
{
    //フォント読み込み
    _fontType = TTF_OpenFont(ttfFilePath.c_str(), _fontSize);
}


FontResource::~FontResource()
{
    Clear();
}


void FontResource::DeleteDefaultFont()
{
    _defaultFont->Clear();
    if (_defaultFont != nullptr)
        _defaultFont = nullptr;
}


void FontResource::SetDefaultType(std::string ttfFilePath, int fontSize)
{
    //フォント読み込み
    _defaultFont = std::make_shared<FontResource>(ttfFilePath, fontSize);
}


void FontResource::Clear()
{
    if (_fontType == nullptr)
        return;
    
    TTF_CloseFont(_fontType);
    _fontType = nullptr;
}