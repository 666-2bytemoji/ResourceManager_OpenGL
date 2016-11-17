#include <stdio.h>
#include "SoundResource.h"

SoundResource::SoundResource(std::string fileName)
    : Resource(fileName)
    , _buffer(0)
{
    //openALのバッファ生成
    Init();
    
    //データファイル読み込み
    if (fileName.find(".wav") != std::string::npos)
        EncodeAsWAV();
    else if (fileName.find(".ogg") != std::string::npos)
        EncodeAsOgg();
}


SoundResource::~SoundResource()
{
    alDeleteBuffers(1, &_buffer);
}


void SoundResource::Init()
{
    alGetError();
    // Generate Buffers
    alGenBuffers(1, &_buffer);
    
    //clear error code
    alGetError();
    ALenum error;
    if ((error = alGetError()) != AL_NO_ERROR)
    {
        printf("alGenBuffersError :%d", error);
    }
}


void SoundResource::EncodeAsWAV()
{
    int channel;
    int bit;
    int size;
    int freq;
    
    // Load wav
    FILE* fp = fopen(_fileName.c_str(),"rb");
    LoadWAV(fp, &channel, &bit, &size, &freq);
    
    //データ読み込み
    unsigned char* _data = new unsigned char[size];
    fread(_data, size, 1, fp);
    
    //バッファへのデータ登録
    ALenum format = AL_FORMAT_STEREO16;
    if(channel == 1)
    {
        if(bit == 8)
        {
            format = AL_FORMAT_MONO8;
        }
        else
        {
            format = AL_FORMAT_MONO16;
        }
    }
    else
    {
        if(bit==8)
        {
            format = AL_FORMAT_STEREO8;
        }
    }
    alBufferData(_buffer, format, _data, size, freq);
    
    fclose(fp);
    delete[] _data;
}


void SoundResource::EncodeAsOgg()
{
    int endian = 0;
    int bitStream = 0;
    long bytes = 0;
    char array[4096];
    
    std::vector<char> bufferData;
    ALenum format = AL_FORMAT_STEREO16; //The sound data format
    
    // Open for binary reading
    FILE *f = fopen(_fileName.c_str(), "rb");
    OggVorbis_File oggFile;
    ov_open(f, &oggFile, NULL, 0);
    
    vorbis_info *pInfo = ov_info(&oggFile, -1);
    
    if (pInfo->channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    
    // The frequency of the sampling rate
    ALsizei freq = pInfo->rate;
    
    do
    {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read
        (
         &oggFile,
         array,
         4096,
         endian,    //エンディアン指定      win:0(little) 1:big
         2,         //WORD単位の指定       win:2(16bit)
         1,         //PCM音声の符号の有無   1:有
         &bitStream //streamingの位置
         );
        
        // Append to end of buffer
        bufferData.insert(bufferData.end(), array, array + bytes);
    }
    while (bytes > 0);
    
    ov_clear(&oggFile);
    fclose(f);
    
    alBufferData(_buffer, format, &bufferData[0]
                 , static_cast<ALsizei>(bufferData.size()), freq);
}


int SoundResource::LoadWAV(FILE* fp, int *channel, int* bit, int *size, int* freq)
{
    short res16;
    int res32;
    int dataSize, chunkSize;
    short channelCnt,bitParSample,blockSize;
    int samplingRate,byteParSec;
    
    int dataPos;
    int flag = 0;
    
    fread(&res32, 4, 1,fp);
    if(res32 != 0x46464952){	//"RIFF"
        return 1;	//error 1
    }
    
    //データサイズ = ファイルサイズ - 8 byte の取得
    fread(&dataSize, 4, 1, fp);
    
    //WAVEヘッダーの読み
    fread(&res32, 4, 1, fp);
    if(res32 != 0x45564157){	//"WAVE"
        return 2;	//error 2
    }
    
    while(flag != 3)
    {
        //チャンクの読み
        fread(&res32, 4, 1, fp);
        fread(&chunkSize, 4, 1, fp);
        
        switch(res32)
        {
            case 0x20746d66:	//"fmt "
                //format 読み込み
                //PCM種類の取得
                fread(&res16, 2, 1, fp);
                if(res16 != 1){
                    //非対応フォーマット
                    return 4;
                }
                //モノラル(1)orステレオ(2)
                fread(&channelCnt, 2, 1, fp);
                if(res16 > 2){
                    //チャンネル数間違い
                    return 5;
                }
                //サンプリングレート
                fread(&samplingRate, 4, 1, fp);
                //データ速度(byte/sec)=サンプリングレート*ブロックサイズ
                fread(&byteParSec, 4, 1, fp);
                //ブロックサイズ(byte/sample)=チャンネル数*サンプルあたりのバイト数
                fread(&blockSize, 2, 1, fp);
                //サンプルあたりのbit数(bit/sample)：8 or 16
                fread(&bitParSample, 2, 1, fp);
                
                *channel = (int)channelCnt;
                *bit = (int)bitParSample;
                *freq = samplingRate;
                
                flag += 1;
                
                break;
            case  0x61746164:	//"data"
                
                *size = chunkSize;
                
                dataPos = ftell(fp);
                
                flag += 2;
                break;
        }
        
    }
    
    //頭出し("fmt "が"data"より後にあった場合のみ動く)
    if (dataPos != ftell(fp)){
        fseek(fp,dataPos,SEEK_SET);
    }
    
    return 0;
}
