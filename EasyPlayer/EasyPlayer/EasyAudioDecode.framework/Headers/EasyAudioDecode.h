#ifndef _EasyAudioDecode_h
#define _EasyAudioDecode_h

typedef struct _HANDLE_ {
    unsigned int code;
    void *pContext;
} EasyAudioHandle;

EasyAudioHandle* EasyAudioDecodeCreate(int code, int sample_rate, int channels, int sample_bit);

int EasyAudioDecode(EasyAudioHandle* pHandle, unsigned char* buffer, int offset, int length, unsigned char* pcm_buffer, int* pcm_length);

void EasyAudioDecodeClose(EasyAudioHandle* pHandle);

#endif