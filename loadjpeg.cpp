#include "loadjpeg.h"
#include <stdlib.h>
#include <stdio.h>

bool JPEG::Load(std::vector<unsigned char>& out, unsigned &sizeX, unsigned &sizeY, std::string fileName)
{
    FILE *File = NULL;
    if ((File = fopen(fileName.c_str(), "rb"))==NULL)
    {
        printf("ファイルがありません");
        return false;
    }
    
    JSAMPARRAY img;
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;
    
    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_decompress( &cinfo );
    
    jpeg_stdio_src( &cinfo, File );
    jpeg_read_header( &cinfo, TRUE );
    sizeX = cinfo.image_width;
    sizeY = cinfo.image_height;
    jpeg_start_decompress( &cinfo );
    
    // イメージを保持するメモリ領域の確保と初期化
    unsigned long size = sizeY*sizeX*3;
    unsigned char Data[size];
    img = (JSAMPARRAY)malloc( sizeof( JSAMPROW ) * cinfo.image_height );
    for ( int i = 0; (unsigned)i < cinfo.image_height; i++ )
    {
        img[i] = (JSAMPROW)calloc( sizeof( JSAMPLE ), 3 * cinfo.image_width );
    }
    
    // 全イメージデータを取得
    while( cinfo.output_scanline < cinfo.output_height )
    {
        jpeg_read_scanlines( &cinfo,img + cinfo.output_scanline,cinfo.output_height - cinfo.output_scanline );
    }
    
    for(int i=0;(unsigned)i<sizeY;i++)
    {
        for(int j=0;(unsigned)j<sizeX*3;j++)
        {
            Data[i*sizeX*3+j] = (char)img[i][j];
        }
    }
    
    jpeg_finish_decompress( &cinfo );
    jpeg_destroy_decompress( &cinfo );
    
    fclose( File );
    
    //イメージデータを保持するメモリ領域を開放
    for (int i = 0; (unsigned)i < sizeY; i++ )
        free(img[i]);
    
    free(img);
    
    out.resize(size);
    for (long i=0;i<size;i++)
    {
        out[i] = Data[i];
    }
    
    return true;
}