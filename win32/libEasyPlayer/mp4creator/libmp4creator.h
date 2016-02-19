#ifndef __LIB_MP4_CREATOR_H__
#define __LIB_MP4_CREATOR_H__


#define LIB_MP4CREATOR_API __declspec(dllexport)


#ifndef MEDIA_TYPE_VIDEO
#define MEDIA_TYPE_VIDEO		0x01
#endif
#ifndef MEDIA_TYPE_AUDIO
#define MEDIA_TYPE_AUDIO		0x02
#endif


#define	VIDEO_CODEC_H264		0x1C
#define AUDIO_CODEC_AAC			0x40

typedef void *MP4C_Handler;


int	LIB_MP4CREATOR_API	MP4C_Init(MP4C_Handler *handler);
int	LIB_MP4CREATOR_API	MP4C_CreateMp4File(MP4C_Handler handler, char *filename, unsigned int _file_buf_size);


int	LIB_MP4CREATOR_API	MP4C_SetMp4VideoInfo(MP4C_Handler handler, unsigned int codec,	unsigned short width, unsigned short height, unsigned int fps);
int	LIB_MP4CREATOR_API	MP4C_SetMp4AudioInfo(MP4C_Handler handler, unsigned int codec,	unsigned int sampleFrequency, unsigned int channel);
int	LIB_MP4CREATOR_API	MP4C_SetH264Sps(MP4C_Handler handler, unsigned short sps_len, unsigned char *sps);
int	LIB_MP4CREATOR_API	MP4C_SetH264Pps(MP4C_Handler handler, unsigned short pps_len, unsigned char *pps);

//int LIB_MP4CREATOR_API		MP4C_SetMp4Info(MP4C_Handler handler, unsigned int codec, unsigned short width, unsigned short height, unsigned int fps);

int	LIB_MP4CREATOR_API	MP4C_AddFrame(MP4C_Handler handler, unsigned int mediatype, unsigned char *pbuf, unsigned int framesize, unsigned char keyframe, unsigned int timestamp_sec, unsigned int timestamp_rtp, unsigned int fps);

unsigned int LIB_MP4CREATOR_API	MP4C_CloseMp4File(MP4C_Handler handler);
int	LIB_MP4CREATOR_API	MP4C_Deinit(MP4C_Handler *handler);



/*
MP4C_Handler	mp4Handle = NULL;

MP4C_Init(&mp4Handle);
MP4C_SetMp4VideoInfo(mp4Handle, VIDEO_CODEC_H264, frameinfo.width, frameinfo.height, frameinfo.fps);
MP4C_SetMp4AudioInfo(mp4Handle, AUDIO_CODEC_AAC, 8000, 1);
MP4C_CreateMp4File(mp4Handle, szMp4Filename, 1024*1024*2);



MP4C_AddFrame(mp4Handle, MEDIA_TYPE_VIDEO, (unsigned char*)pbuf, frameinfo.length, frameinfo.type, frameinfo.timestamp_sec, frameinfo.rtptimestamp, pts);


MP4C_CloseMp4File(mp4Handle);
MP4C_Deinit(&mp4Handle);
	
*/


#endif
