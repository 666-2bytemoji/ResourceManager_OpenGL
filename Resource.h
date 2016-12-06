#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource
{
public:
    
    Resource(std::string fileName);
    virtual ~Resource();
    
    //リソースのロード
    virtual bool Load();
    //リソースの削除
    virtual bool Unload();
    
    std::string GetName() const { return _fileName; }
    
protected:
    
    //リソースを読み込むファイル名
    std::string _fileName;
};

#endif