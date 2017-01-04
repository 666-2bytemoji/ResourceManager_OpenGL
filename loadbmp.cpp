#include "loadbmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fstream>


bool BMP::Load(std::vector<unsigned char>& out, unsigned &sizeX, unsigned &sizeY, std::string fileName)
{
    FILE *File;

    //画像のバイトサイズ
    unsigned long size;
    
    unsigned long i;
    
    //ファイルオープン
    if ((File = fopen(fileName.c_str(), "rb"))==NULL)
    {
        printf("ファイルがありません");
        return false;
    }
    
    //ビットマップの幅データ部分へ移動
    fseek(File, 18, SEEK_CUR);
    //横幅を読み込む
    if ((i = fread(&sizeX, 4, 1, File)) != 1)
    {
        printf("読み込みエラー");
        fclose(File);
        return false;
    }
    //縦幅を読み込む
    if ((i = fread(&sizeY, 4, 1, File)) != 1)
    {
        printf("読み込みエラー");
        fclose(File);
        return false;
    }
    
    //画像サイズの計算
    size = sizeX * sizeY * 3;

    //デバイス面数
    unsigned short int planes;
    //プレーン数を読み込む(bmpだと1になる
    if ((fread(&planes, sizeof(unsigned short int), 1, File)) != 1)
    {
        printf("プレーン数が読み込めません");
        fclose(File);
        return false;
    }
    
    if (planes != 1)
    {
        printf("プレーン数が1以外です");
        fclose(File);
        return false;
    }
    
    
    //ピクセル数
    unsigned short int bpp;
    //ピクセル値を読み込む
    if ((i = fread(&bpp, sizeof(unsigned short int), 1, File)) != 1)
    {
        printf("ビット数が読めません");
        fclose(File);
        return false;
    }
    
    //TODO : 32bppに対応
    //24bppでなければエラー
    if (bpp != 24)
    {
        printf("24ビット画像ではありません");
        fclose(File);
        return false;
    }
    
    //24ビット飛ばして、カラーデータ(RGB)部分へ
    fseek(File, 24, SEEK_CUR);

    //データ読み込み
    unsigned char Data[size];
    i = fread(&Data, sizeof(unsigned char), size, File);
    if (i == 0)
    {
        printf("データが読めません");
        fclose(File);
        return false;
    }
    
    //bgr -> rgb
    unsigned char temp;
    out.resize(size);
    for (i=0;i<size;i+=3)
    {
        temp = Data[i];
        Data[i] = Data[i+2];
        Data[i+2] = temp;
        
        out[i] = Data[i];
        out[i + 1] = Data[i + 1];
        out[i + 2] = Data[i + 2];
    }
    
    //上下左右反転
    /*
    for (i=0;i<size;i+=3)
    {
        out[i] = Data[size - 3 - i];
        out[i + 1] = Data[size - 3 - i + 1];
        out[i + 2] = Data[size - 3 - i + 2];
    }
     */
    
    //左右反転
    for (unsigned long y=1; y<sizeY; y++)
    {
        for (unsigned long x=0;x<sizeX;x++)
        {
            for (unsigned long k=0;k<3;k++)
            {
                unsigned long inp = (y * sizeX + x) * 3 + k;
                unsigned long outp = size - 3 - (y * sizeX) * 3 + x * 3 + k;
                out[inp] = Data[outp];
            }
        }
    }
    fclose(File);

    return true;
}