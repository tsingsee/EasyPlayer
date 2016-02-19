#include "ChannelManager.h"
#include <time.h>
#include "vstime.h"
#include "trace.h"
#include "CreateDump.h"

#define		CHANNEL_ID_GAIN			1000


#define	__DELETE_ARRAY(x)	{if (NULL!=x) {delete []x;x=NULL;}}

int CALLBACK __NVSourceCallBack( int _chid, int *_chPtr, int _mediatype, char *pbuf, NVS_FRAME_INFO *frameinfo);

CChannelManager	*pChannelManager = NULL;
CChannelManager::CChannelManager(void)
{
	pRealtimePlayThread	=	NULL;
	pAudioPlayThread	=	NULL;
	memset(&d3dAdapter, 0x00, sizeof(D3D_ADAPTER_T));

	InitializeCriticalSection(&crit);
}


CChannelManager::~CChannelManager(void)
{
	Release();
	DeleteCriticalSection(&crit);
}

int	CChannelManager::Initial()
{
	if (NULL == pRealtimePlayThread)
	{
		pRealtimePlayThread = new PLAY_THREAD_OBJ[MAX_CHANNEL_NUM];
		if (NULL == pRealtimePlayThread)		return -1;

		memset(&pRealtimePlayThread[0], 0x00, sizeof(PLAY_THREAD_OBJ)*MAX_CHANNEL_NUM);

		for (int i=0; i<MAX_CHANNEL_NUM; i++)
		{
			InitializeCriticalSection(&pRealtimePlayThread[i].crit);
			pRealtimePlayThread[i].renderFormat = GDI_FORMAT_RGB24;		//默认为GDI显示
			pRealtimePlayThread[i].channelId = i+1;
		}
	}

	if (NULL == pChannelManager)		pChannelManager	=	this;

	return 0;
}

void CChannelManager::Release()
{
	if (NULL != pRealtimePlayThread)
	{
		for (int i=0; i<MAX_CHANNEL_NUM; i++)
		{
			if (NULL != pRealtimePlayThread[i].nvsHandle)
			{
				EasyNVS_CloseStream(pRealtimePlayThread[i].nvsHandle);
				EasyNVS_Deinit(&pRealtimePlayThread[i].nvsHandle);
			}

			ClosePlayThread(&pRealtimePlayThread[i]);
			DeleteCriticalSection(&pRealtimePlayThread[i].crit);
		}
		__DELETE_ARRAY(pRealtimePlayThread);
	}
	//销毁音频播放线程
	if (NULL != pAudioPlayThread)
	{
		if (NULL != pAudioPlayThread->pSoundPlayer)
		{
			pAudioPlayThread->pSoundPlayer->Close();
			delete pAudioPlayThread->pSoundPlayer;
			pAudioPlayThread->pSoundPlayer = NULL;
		}
		delete pAudioPlayThread;
		pAudioPlayThread = NULL;
	}
}


int	CChannelManager::OpenStream(const char *url, HWND hWnd, RENDER_FORMAT renderFormat, int _rtpovertcp, const char *username, const char *password)
{
	if (NULL == pRealtimePlayThread)			return -1;
	if ( (NULL == url) || (0==strcmp(url, "\0")))		return -1;

	int iNvsIdx = -1;
	EnterCriticalSection(&crit);
	do
	{
		for (int i=0; i<MAX_CHANNEL_NUM; i++)
		{
			if (NULL == pRealtimePlayThread[i].nvsHandle)
			{
				iNvsIdx = i;
				break;
			}
		}

		if (iNvsIdx == -1)		break;

		EasyNVS_Init(&pRealtimePlayThread[iNvsIdx].nvsHandle);
		if (NULL == pRealtimePlayThread[iNvsIdx].nvsHandle)		break;	//退出while循环

		unsigned int mediaType = MEDIA_TYPE_VIDEO;
		mediaType |= MEDIA_TYPE_AUDIO;		//换为NVSource, 屏蔽声音
		EasyNVS_SetCallback(pRealtimePlayThread[iNvsIdx].nvsHandle, __NVSourceCallBack);
		EasyNVS_OpenStream(pRealtimePlayThread[iNvsIdx].nvsHandle, iNvsIdx, (char*)url, _rtpovertcp==0x01?RTP_OVER_TCP:RTP_OVER_UDP, mediaType, (char*)username, (char*)password, (int*)&pRealtimePlayThread[iNvsIdx], 1000, 0);

		pRealtimePlayThread[iNvsIdx].hWnd = hWnd;
		pRealtimePlayThread[iNvsIdx].renderFormat = (D3D_SUPPORT_FORMAT)renderFormat;
		CreatePlayThread(&pRealtimePlayThread[iNvsIdx]);

	}while (0);
	LeaveCriticalSection(&crit);

	if (iNvsIdx >= 0)	iNvsIdx += CHANNEL_ID_GAIN;
	return iNvsIdx;
}

void CChannelManager::CloseStream(int channelId)
{
	if (NULL == pRealtimePlayThread)			return;

	int iNvsIdx = channelId - CHANNEL_ID_GAIN;
	if (iNvsIdx < 0 || iNvsIdx>= MAX_CHANNEL_NUM)	return;

	EnterCriticalSection(&crit);

	//关闭rtsp client
	EasyNVS_CloseStream(pRealtimePlayThread[iNvsIdx].nvsHandle);
	EasyNVS_Deinit(&pRealtimePlayThread[iNvsIdx].nvsHandle);
	//关闭播放线程
	ClosePlayThread(&pRealtimePlayThread[iNvsIdx]);

	LeaveCriticalSection(&crit);
}


int	CChannelManager::ShowStatisticalInfo(int channelId, int _show)
{
	if (NULL == pRealtimePlayThread)			return -1;

	int iNvsIdx = channelId - CHANNEL_ID_GAIN;
	if (iNvsIdx < 0 || iNvsIdx>= MAX_CHANNEL_NUM)	return -1;

	pRealtimePlayThread[iNvsIdx].showStatisticalInfo = _show;
	return 0;
}
int	CChannelManager::SetFrameCache(int channelId, int _cache)
{
	if (NULL == pRealtimePlayThread)			return -1;

	int iNvsIdx = channelId - CHANNEL_ID_GAIN;
	if (iNvsIdx < 0 || iNvsIdx>= MAX_CHANNEL_NUM)	return -1;

	pRealtimePlayThread[iNvsIdx].frameCache = _cache;
	return 0;
}
int	CChannelManager::SetShownToScale(int channelId, int ShownToScale)
{
	if (NULL == pRealtimePlayThread)			return -1;

	int iNvsIdx = channelId - CHANNEL_ID_GAIN;
	if (iNvsIdx < 0 || iNvsIdx>= MAX_CHANNEL_NUM)	return -1;

	pRealtimePlayThread[iNvsIdx].ShownToScale = ShownToScale;
	return 0;
}

//播放声音		2014.12.01
int	CChannelManager::PlaySound(int channelId)
{
	int ret = -1;
	if (NULL == pAudioPlayThread)
	{
		pAudioPlayThread = new AUDIO_PLAY_THREAD_OBJ;
		if (NULL == pAudioPlayThread)		return ret;
		memset(pAudioPlayThread, 0x00, sizeof(AUDIO_PLAY_THREAD_OBJ));
	}

	ClearAllSoundData();		//如果当前正在播放其它音频,则清空


	int iNvsIdx = channelId - CHANNEL_ID_GAIN + 1;

	if (pAudioPlayThread->channelId != iNvsIdx)
	{
		pAudioPlayThread->channelId = iNvsIdx;//channelId;
	
		if (NULL != pAudioPlayThread->pSoundPlayer)
		{
			pAudioPlayThread->pSoundPlayer->Close();
		}

		pAudioPlayThread->audiochannels	=	0;
		pAudioPlayThread->samplerate	=	0;
		pAudioPlayThread->bitpersample	=	0;
		ret = 0;
	}

	return ret;
}
int	CChannelManager::StopSound()
{
	if (NULL == pAudioPlayThread)		return 0;

	if (NULL != pAudioPlayThread->pSoundPlayer)
	{
		pAudioPlayThread->pSoundPlayer->Close();
	}
	pAudioPlayThread->audiochannels  = 0;
	pAudioPlayThread->channelId = -1;
	return 0;
}
void CChannelManager::ClearAllSoundData()
{
	if (NULL != pChannelManager)
	{
		if (NULL != pChannelManager->pAudioPlayThread)
		{
			if (NULL != pChannelManager->pAudioPlayThread->pSoundPlayer)
			{
				pChannelManager->pAudioPlayThread->pSoundPlayer->Clear();
			}
		}
	}
}

void CChannelManager::CreatePlayThread(PLAY_THREAD_OBJ	*_pPlayThread)
{
	if (NULL == _pPlayThread)		return;

	if (_pPlayThread->decodeThread.flag == 0x00)
	{
		//_pPlayThread->ch_tally = 0;
		_pPlayThread->decodeThread.flag = 0x01;
		_pPlayThread->decodeThread.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_lpDecodeThread, _pPlayThread, 0, NULL);
		while (_pPlayThread->decodeThread.flag!=0x02 && _pPlayThread->decodeThread.flag!=0x00)	{Sleep(100);}
		if (NULL != _pPlayThread->decodeThread.hThread)
		{
			SetThreadPriority(_pPlayThread->decodeThread.hThread, THREAD_PRIORITY_HIGHEST);
		}
	}
	if (_pPlayThread->displayThread.flag == 0x00)
	{
		_pPlayThread->displayThread.flag = 0x01;
		_pPlayThread->displayThread.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_lpDisplayThread, _pPlayThread, 0, NULL);
		while (_pPlayThread->displayThread.flag!=0x02 && _pPlayThread->displayThread.flag!=0x00)	{Sleep(100);}
		if (NULL != _pPlayThread->displayThread.hThread)
		{
			SetThreadPriority(_pPlayThread->displayThread.hThread, THREAD_PRIORITY_HIGHEST);
		}
	}
}
void CChannelManager::ClosePlayThread(PLAY_THREAD_OBJ	*_pPlayThread)
{
	if (NULL == _pPlayThread)		return;

	//关闭解码线程
	if (_pPlayThread->decodeThread.flag != 0x00)
	{
#ifdef _DEBUG
		_TRACE("关闭播放线程[%d]\n", _pPlayThread->channelId);
#endif
		_pPlayThread->decodeThread.flag = 0x03;
		while (_pPlayThread->decodeThread.flag!=0x00)	{Sleep(100);}
	}
	if (NULL != _pPlayThread->decodeThread.hThread)
	{
		CloseHandle(_pPlayThread->decodeThread.hThread);
		_pPlayThread->decodeThread.hThread = NULL;
	}
	for (int i=0; i<MAX_DECODER_NUM; i++)		//关闭解码器
	{
		if (NULL != _pPlayThread->decoderObj[i].ffDecoder)
		{
			FFD_Deinit(&_pPlayThread->decoderObj[i].ffDecoder);
			_pPlayThread->decoderObj[i].ffDecoder = NULL;
		}
		memset(&_pPlayThread->decoderObj[i].codec, 0x00, sizeof(CODEC_T));
		_pPlayThread->decoderObj[i].yuv_size = 0;
	}

	//关闭播放线程
	if (_pPlayThread->displayThread.flag != 0x00)
	{
		_pPlayThread->displayThread.flag = 0x03;
		while (_pPlayThread->displayThread.flag!=0x00)	{Sleep(100);}
	}
	if (NULL != _pPlayThread->displayThread.hThread)
	{
		CloseHandle(_pPlayThread->displayThread.hThread);
		_pPlayThread->displayThread.hThread = NULL;
	}
	for (int i=0; i<MAX_YUV_FRAME_NUM; i++)
	{
		__DELETE_ARRAY(_pPlayThread->yuvFrame[i].pYuvBuf);
		memset(&_pPlayThread->yuvFrame[i].frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));
		_pPlayThread->yuvFrame[i].Yuvsize = 0;
	}

	//释放队列
	if (NULL != _pPlayThread->pAVQueue)
	{
		SSQ_Deinit(_pPlayThread->pAVQueue);
		delete _pPlayThread->pAVQueue;
		_pPlayThread->pAVQueue = NULL;
	}
	_pPlayThread->initQueue = 0x00;
	_pPlayThread->frameCache	=	NULL;

	memset(&_pPlayThread->decodeThread, 0x00, sizeof(THREAD_OBJ));
	memset(&_pPlayThread->displayThread, 0x00, sizeof(THREAD_OBJ));
	_pPlayThread->hWnd	=	NULL;
}



int	CChannelManager::SetAudioParams(unsigned int _channel, unsigned int _samplerate, unsigned int _bitpersample)
{
	if (NULL == pAudioPlayThread)		return -1;

	WAVEFORMATEX wfx = {0,};
	wfx.cbSize = sizeof(WAVEFORMATEX);
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nSamplesPerSec = _samplerate;
	wfx.wBitsPerSample = _bitpersample;
	wfx.nChannels = _channel;
	wfx.nBlockAlign =  wfx.nChannels * ( wfx.wBitsPerSample / 8 );//(wfx.wBitsPerSample*wfx.nChannels)>>3; //wfx.nChannels * ( wfx.wBitsPerSample / 8 );
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	wfx.nAvgBytesPerSec	= wfx.nSamplesPerSec * wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nBlockAlign		= wfx.nChannels * wfx.wBitsPerSample / 8;

	if (NULL == pAudioPlayThread->pSoundPlayer)
	{
		pAudioPlayThread->pSoundPlayer = new CSoundPlayer();
	}
	if (NULL != pAudioPlayThread->pSoundPlayer)
	{
		pAudioPlayThread->pSoundPlayer->Close();
		pAudioPlayThread->pSoundPlayer->Open(wfx);
	}
	pAudioPlayThread->audiochannels	=	_channel;
	return 0;
}

void ParseDecoder2Render(int *_decoder, int _width, int _height, int renderFormat, int *yuvsize)
{
	if (NULL == _decoder)		return;
	int nDecoder = 0x00;
	int nYUVSize = 0x00;
	if (renderFormat == D3D_FORMAT_UYVY)
	{
		nDecoder	=	17;	//PIX_FMT_UYVY422
		nYUVSize	=	_width*_height*2+1;
	}
	else if (renderFormat == D3D_FORMAT_YV12)
	{
		nDecoder	=	0;//OUTPUT_PIX_FMT_YUV420P;
		nYUVSize	=	_width*_height*3/2+1;
	}
	else if (renderFormat == D3D_FORMAT_YUY2)
	{
		nDecoder	=	1;//OUTPUT_PIX_FMT_YUV422;
		nYUVSize	=	_width*_height*2+1;
	}
	else if (renderFormat == D3D_FORMAT_RGB565)				//23 ok
	{
		nDecoder	=	44;
		nYUVSize	=	_width*_height*2+1;
	}
	else if (renderFormat == D3D_FORMAT_RGB555)				//23 ok
	{
		nDecoder	=	46;
		nYUVSize	=	_width*_height*2+1;
	}
	else if (renderFormat == D3D_FORMAT_X8R8G8B8)
	{
		nDecoder	=	30;
		nYUVSize	=	_width*_height*4+1;
	}
	else if (renderFormat == D3D_FORMAT_A8R8G8B8)			//21 ok
	{
		nDecoder	=	30;
		nYUVSize	=	_width*_height*4+1;
	}
	else if (renderFormat == (D3D_SUPPORT_FORMAT)22)		//22	ok
	{
		nDecoder	=	28;
		nYUVSize	=	_width*_height*4+1;
	}
	else if (renderFormat == (D3D_SUPPORT_FORMAT)1498831189)
	{
		nDecoder	=	17;
		nYUVSize	=	_width*_height*4+1;
	}
	else if (renderFormat == (D3D_SUPPORT_FORMAT)36)
	{
		nDecoder	=	32;
		nYUVSize	=	_width*_height*4+1;
	}
	else if (renderFormat == GDI_FORMAT_RGB24)
	{
		nDecoder	=	3;
		nYUVSize	=	_width*_height*3+1;
	}
	else
	{
		nDecoder	=	28;
		nYUVSize	=	_width*_height*4+1;
	}

	if (NULL != _decoder)	*_decoder = nDecoder;
	if (NULL != yuvsize)	*yuvsize  = nYUVSize;
}

DECODER_OBJ	*GetDecoder(PLAY_THREAD_OBJ	*_pPlayThread, unsigned int mediaType, MEDIA_FRAME_INFO *_frameinfo)
{
	if (NULL == _pPlayThread || NULL==_frameinfo)									return NULL;


	int iIdx = -1;
	int iExist = -1;
	for (int i=0; i<MAX_DECODER_NUM; i++)
	{
		if (MEDIA_TYPE_VIDEO == mediaType)
		{
			if (_frameinfo->width < 1 || _frameinfo->height<1 || _frameinfo->codec<1)		return NULL;

			if (_pPlayThread->decoderObj[i].codec.vidCodec == 0x00 &&
				_pPlayThread->decoderObj[i].codec.width == 0x00 &&
				_pPlayThread->decoderObj[i].codec.height== 0x00 && iIdx==-1)
			{
				iIdx = i;
			}

			if (_pPlayThread->decoderObj[i].codec.vidCodec == _frameinfo->codec &&
				_pPlayThread->decoderObj[i].codec.width == _frameinfo->width &&
				_pPlayThread->decoderObj[i].codec.height== _frameinfo->height)
			{
				if (NULL == _pPlayThread->decoderObj[i].ffDecoder)
				{
					iExist = i;
					int nDecoder = OUTPUT_PIX_FMT_YUV420P;
					//ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, _pPlayThread->renderFormat, &_pPlayThread->decoderObj[i].yuv_size);

					if (_pPlayThread->renderFormat == GDI_FORMAT_RGB24)
					{
						ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, _pPlayThread->renderFormat, &_pPlayThread->decoderObj[i].yuv_size);
					}
					else
					{
#ifdef __ENABLE_SSE
						ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, DISPLAY_FORMAT_YV12, &_pPlayThread->decoderObj[i].yuv_size);
#else
						ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, _pPlayThread->renderFormat, &_pPlayThread->decoderObj[i].yuv_size);
#endif
					}

					FFD_Init(&_pPlayThread->decoderObj[i].ffDecoder);
					FFD_SetVideoDecoderParam(_pPlayThread->decoderObj[i].ffDecoder, _frameinfo->width, _frameinfo->height, _frameinfo->codec, nDecoder);
				}
				return &_pPlayThread->decoderObj[i];
			}

			if (iIdx>=0)
			{
				if (NULL == _pPlayThread->decoderObj[iIdx].ffDecoder)
				{
					int nDecoder = OUTPUT_PIX_FMT_YUV420P;
					if (_pPlayThread->renderFormat == GDI_FORMAT_RGB24)
					{
						ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, _pPlayThread->renderFormat, &_pPlayThread->decoderObj[iIdx].yuv_size);
					}
					else
					{
#ifdef __ENABLE_SSE
						ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, DISPLAY_FORMAT_YV12, &_pPlayThread->decoderObj[iIdx].yuv_size);
#else
						ParseDecoder2Render(&nDecoder, _frameinfo->width, _frameinfo->height, _pPlayThread->renderFormat, &_pPlayThread->decoderObj[iIdx].yuv_size);
#endif
					}

					FFD_Init(&_pPlayThread->decoderObj[iIdx].ffDecoder);
					FFD_SetVideoDecoderParam(_pPlayThread->decoderObj[iIdx].ffDecoder, _frameinfo->width, _frameinfo->height, _frameinfo->codec, nDecoder);

					if (NULL != _pPlayThread->decoderObj[iIdx].ffDecoder)
					{
						EnterCriticalSection(&_pPlayThread->crit);
						_pPlayThread->resetD3d	=	true;
						LeaveCriticalSection(&_pPlayThread->crit);

						_pPlayThread->decoderObj[iIdx].codec.vidCodec	= _frameinfo->codec;
						_pPlayThread->decoderObj[iIdx].codec.width	= _frameinfo->width;
						_pPlayThread->decoderObj[iIdx].codec.height = _frameinfo->height;

						return &_pPlayThread->decoderObj[iIdx];
					}
					else
					{
						return NULL;
					}
				}
			}
		}
		else if (MEDIA_TYPE_AUDIO == mediaType)
		{
			if (_frameinfo->sample_rate < 1 || _frameinfo->channels<1 || _frameinfo->codec<1)		return NULL;

			if (_pPlayThread->decoderObj[i].codec.audCodec == 0x00 &&
				_pPlayThread->decoderObj[i].codec.samplerate == 0x00 &&
				_pPlayThread->decoderObj[i].codec.channels== 0x00 && iIdx==-1)
			{
				iIdx = i;
			}

			if (_pPlayThread->decoderObj[i].codec.audCodec == _frameinfo->codec &&
				_pPlayThread->decoderObj[i].codec.samplerate == _frameinfo->sample_rate &&
				_pPlayThread->decoderObj[i].codec.channels== _frameinfo->channels)
			{
				if (NULL == _pPlayThread->decoderObj[i].ffDecoder)
				{
					iExist = i;

					FFD_Init(&_pPlayThread->decoderObj[i].ffDecoder);
					FFD_SetAudioDecoderParam(_pPlayThread->decoderObj[i].ffDecoder, _frameinfo->channels, _frameinfo->sample_rate, _frameinfo->codec);
				}
				return &_pPlayThread->decoderObj[i];
			}

			if (iIdx>=0)
			{
				if (NULL == _pPlayThread->decoderObj[iIdx].ffDecoder)
				{
					FFD_Init(&_pPlayThread->decoderObj[iIdx].ffDecoder);
					FFD_SetAudioDecoderParam(_pPlayThread->decoderObj[i].ffDecoder, _frameinfo->channels, _frameinfo->sample_rate, _frameinfo->codec);

					if (NULL != _pPlayThread->decoderObj[iIdx].ffDecoder)
					{
						_pPlayThread->decoderObj[iIdx].codec.audCodec	= _frameinfo->codec;
						_pPlayThread->decoderObj[iIdx].codec.samplerate	= _frameinfo->sample_rate;
						_pPlayThread->decoderObj[iIdx].codec.channels = _frameinfo->channels;

						return &_pPlayThread->decoderObj[iIdx];
					}
					else
					{
						return NULL;
					}
				}
				else
				{
					FFD_SetAudioDecoderParam(_pPlayThread->decoderObj[i].ffDecoder, _frameinfo->channels, _frameinfo->sample_rate, _frameinfo->codec);
					_pPlayThread->decoderObj[iIdx].codec.audCodec	= _frameinfo->codec;
					_pPlayThread->decoderObj[iIdx].codec.samplerate	= _frameinfo->sample_rate;
					_pPlayThread->decoderObj[iIdx].codec.channels = _frameinfo->channels;

					return &_pPlayThread->decoderObj[iIdx];
				}
			}
		}
	}

	return NULL;
}



//异常处理函数
LONG CrashHandler_Decode(EXCEPTION_POINTERS *pException)
{
	SYSTEMTIME	systemTime;
	GetLocalTime(&systemTime);

	wchar_t wszFile[MAX_PATH] = {0,};
	wsprintf(wszFile, TEXT("decode_%04d%02d%02d %02d%02d%02d.dmp"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	CreateDumpFile(wszFile, pException);

	return EXCEPTION_EXECUTE_HANDLER;		//返回值EXCEPTION_EXECUTE_HANDLER	EXCEPTION_CONTINUE_SEARCH	EXCEPTION_CONTINUE_EXECUTION
}
LONG CrashHandler_Display(EXCEPTION_POINTERS *pException)
{
	SYSTEMTIME	systemTime;
	GetLocalTime(&systemTime);

	wchar_t wszFile[MAX_PATH] = {0,};
	wsprintf(wszFile, TEXT("display_%04d%02d%02d %02d%02d%02d.dmp"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	CreateDumpFile(wszFile, pException);

	return EXCEPTION_EXECUTE_HANDLER;		//返回值EXCEPTION_EXECUTE_HANDLER	EXCEPTION_CONTINUE_SEARCH	EXCEPTION_CONTINUE_EXECUTION
}


LPTHREAD_START_ROUTINE CChannelManager::_lpDecodeThread( LPVOID _pParam )
{
	PLAY_THREAD_OBJ *pThread = (PLAY_THREAD_OBJ*)_pParam;
	if (NULL == pThread)			return 0;

	pThread->decodeThread.flag	=	0x02;

	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashHandler_Decode);

#ifdef _DEBUG
	_TRACE("解码线程[%d]已启动. ThreadId:%d ...\n", pThread->channelId, GetCurrentThreadId());
#endif

	//SetThreadAffinityMask(GetCurrentThread(), 1);

	if (NULL != pThread->yuvFrame)
	{
		for (int i=0; i<MAX_YUV_FRAME_NUM; i++)
		{
			memset(&pThread->yuvFrame[i].frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));
		}
	}
	unsigned int channelid = 0;
	unsigned int mediatype = 0;
	MEDIA_FRAME_INFO	frameinfo;
	int buf_size = 1024*1024;
	char *pbuf = new char[buf_size];
	if (NULL == pbuf)
	{
		pThread->decodeThread.flag	=	0x00;
		return 0;
	}
	memset(pbuf, 0x00, buf_size);

	int tmp_fps = 0;	//临时统计帧率		add by gavin 2014.11.19
	int tmp_fps_total = 0;
	int tmp_usage_time = 0;
	unsigned int tmp_fps_timestamp = 0;

	pThread->decodeYuvIdx	=	0;
	memset(&frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));

	//#define AVCODEC_MAX_AUDIO_FRAME_SIZE	(192000)
#define AVCODEC_MAX_AUDIO_FRAME_SIZE	(64000)
	int audbuf_len = (AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2;
	unsigned char *audio_buf = new unsigned char[audbuf_len+1];
	memset(audio_buf, 0x00, audbuf_len);

	while (1)
	{
		if (pThread->decodeThread.flag == 0x03)		break;

		if (pThread->initQueue == 0x00 || NULL==pThread->pAVQueue)
		{
			Sleep(1);
			continue;
		}

		int ret = SSQ_GetData(pThread->pAVQueue, &channelid, &mediatype, &frameinfo, pbuf);
		if (ret < 0)
		{
			_VS_BEGIN_TIME_PERIOD(1);
			__VS_Delay(1);
			_VS_END_TIME_PERIOD(1);
			continue;
		}

		if (mediatype == MEDIA_TYPE_VIDEO)
		{
#if 1		//2秒统计一次帧率
			unsigned int uiTmp = (unsigned int)time(NULL);
			if (uiTmp - tmp_fps_timestamp >= 2)
			{
				tmp_fps_total = tmp_fps/2;
				if (tmp_fps_total % 5 != 0X00)	tmp_fps_total++;
				tmp_fps = 1;

				tmp_fps_timestamp = uiTmp;
			}
			else
			{
				tmp_fps ++;
			}

			if (tmp_fps_total > 0)			frameinfo.fps = tmp_fps_total;
#endif


#ifdef _DEBUG1
			_TRACE("解码线程[%d]解码...%d   %d x %d\n", pThread->id, channelid, frameinfo.width, frameinfo.height);
#endif
			//==============================================

			//_TRACE("DECODE queue: %d\n", pChannelObj->pQueue->pQueHeader->videoframes);
			if (pThread->frameQueue > MAX_CACHE_FRAME)
			{
				//_TRACE("[ch%d]缓存帧数[%d]>设定帧数[%d].  清空队列并等待下一个Key frame.\n", pThread->renderCh, pThread->framequeue, MAX_CACHE_FRAME);
				_TRACE("[ch%d]缓存帧数[%d]>设定帧数[%d].  清空队列并等待下一个Key frame.\n", pThread->channelId, pThread->frameQueue, MAX_CACHE_FRAME);

				SSQ_Clear(pThread->pAVQueue);
				pThread->findKeyframe = 0x01;
				pThread->frameQueue = pThread->pAVQueue->pQueHeader->videoframes;
				continue;

				EnterCriticalSection(&pThread->crit);
				SSQ_Clear(pThread->pAVQueue);
				pThread->rtpTimestamp = 0;
				pThread->decodeYuvIdx = 0;
				pThread->findKeyframe = 0x01;
				for (int i=0; i<MAX_YUV_FRAME_NUM; i++)
				{
					memset(&pThread->yuvFrame[i].frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));
				}
				LeaveCriticalSection(&pThread->crit);
			}

			if ( (pThread->findKeyframe==0x01) && (frameinfo.type==0x01) )
			{
				pThread->findKeyframe = 0x00;
			}
			else if (pThread->findKeyframe==0x01)
			{
#ifdef _DEBUG
				//_TRACE("[ch%d]Find Keyframe..\n", pThread->renderCh);
				_TRACE("[ch%d]Find Keyframe..\n", pThread->channelId);
#endif
				continue;
			}

			DECODER_OBJ *pDecoderObj = GetDecoder(pThread, MEDIA_TYPE_VIDEO, &frameinfo);		//获取相应的解码器
			if (NULL == pDecoderObj)
			{
#ifdef _DEBUG
				_TRACE("[ch%d]获取解码器失败.\n", pThread->channelId);
#endif
				_VS_BEGIN_TIME_PERIOD(1);
				__VS_Delay(1);
				_VS_END_TIME_PERIOD(1);
				continue;
			}

			//如果已申请的内存小于当前需要的内存大小,则重新申请
			if ( (pThread->yuvFrame[pThread->decodeYuvIdx].Yuvsize < pDecoderObj->yuv_size) && (NULL != pThread->yuvFrame[pThread->decodeYuvIdx].pYuvBuf))
			{
				__DELETE_ARRAY(pThread->yuvFrame[pThread->decodeYuvIdx].pYuvBuf);
			}
			if (NULL == pThread->yuvFrame[pThread->decodeYuvIdx].pYuvBuf)
			{
				pThread->yuvFrame[pThread->decodeYuvIdx].Yuvsize = pDecoderObj->yuv_size;//MAX_FRAME_SIZE;//pThread->yuv_size;
				pThread->yuvFrame[pThread->decodeYuvIdx].pYuvBuf = new char[pThread->yuvFrame[pThread->decodeYuvIdx].Yuvsize];
			}
			if (NULL == pThread->yuvFrame[pThread->decodeYuvIdx].pYuvBuf)		continue;

			while (pThread->yuvFrame[pThread->decodeYuvIdx].frameinfo.length > 0)		//等待该帧显示完成
			{
				if (pThread->decodeThread.flag == 0x03)		break;

				_VS_BEGIN_TIME_PERIOD(1);
				__VS_Delay(1);
				_VS_END_TIME_PERIOD(1);
			}
			if (pThread->decodeThread.flag == 0x03)		break;


			//手动录像
			if (NULL != pThread->mp4cHandle)
			{
				//30fps * 60 seconds * 30 minutes  = 手动录像必须小于30分钟
				if (pThread->vidFrameNum >= 30*60*30 || (pThread->manuRecording == 0x00) )
				{
					MP4C_CloseMp4File(pThread->mp4cHandle);
					MP4C_Deinit(&pThread->mp4cHandle);
					pThread->mp4cHandle = NULL;
					pThread->vidFrameNum = 0;
				}
				else
				{
					pThread->vidFrameNum ++;
					MP4C_AddFrame(pThread->mp4cHandle, MEDIA_TYPE_VIDEO, (unsigned char*)pbuf, frameinfo.length, frameinfo.type, frameinfo.timestamp_sec, frameinfo.rtptimestamp, frameinfo.fps);
				}
			}
			else if (pThread->manuRecording == 0x01 && NULL==pThread->mp4cHandle)
			{
				unsigned int timestamp = (unsigned int)time(NULL);
				time_t tt = timestamp;
				struct tm *_time = localtime(&tt);
				char szTime[64] = {0,};
				strftime(szTime, 32, "%Y%m%d %H%M%S", _time);

				memset(pThread->manuRecordingFile, 0x00, sizeof(pThread->manuRecordingFile));
				sprintf(pThread->manuRecordingFile, "ch%d_%s.mp4", pThread->channelId, szTime);
				//sprintf(pThread->manuRecordingFile, "ch%d.mp4", pThread->channelId);

				MP4C_Init(&pThread->mp4cHandle);
				MP4C_SetMp4VideoInfo(pThread->mp4cHandle, VIDEO_CODEC_H264, frameinfo.width, frameinfo.height, frameinfo.fps);
				MP4C_SetMp4AudioInfo(pThread->mp4cHandle, AUDIO_CODEC_AAC, 8000, 1);
				MP4C_CreateMp4File(pThread->mp4cHandle, pThread->manuRecordingFile, 1024*1024*2);
			}

			//解码
			EnterCriticalSection(&pThread->crit);
			if (0 != FFD_DecodeVideo3(pDecoderObj->ffDecoder, pbuf, frameinfo.length, pThread->yuvFrame[pThread->decodeYuvIdx].pYuvBuf, frameinfo.width, frameinfo.height))
			{
				_TRACE("解码失败... framesize:%d   %02X %02X %02X %02X %02X\n", frameinfo.length, (unsigned char)pbuf[0], (unsigned char)pbuf[1], (unsigned char)pbuf[2], (unsigned char)pbuf[3], (unsigned char)pbuf[4]);

				if (frameinfo.type == 0x01)		//关键帧
				{
					_TRACE("[ch%d]当前关键帧解码失败...\n", pThread->channelId);
				}
				pThread->findKeyframe = 0x01;
			}
			else
			{
				memcpy(&pThread->yuvFrame[pThread->decodeYuvIdx].frameinfo, &frameinfo, sizeof(MEDIA_FRAME_INFO));

				pThread->decodeYuvIdx ++;
				if (pThread->decodeYuvIdx >= MAX_YUV_FRAME_NUM)		pThread->decodeYuvIdx = 0;
			}
			LeaveCriticalSection(&pThread->crit);
		}
		else if (MEDIA_TYPE_AUDIO == mediatype)		//音频
		{
#if 1
			if (NULL != pChannelManager)
			{
				if (NULL != pChannelManager->pAudioPlayThread && pChannelManager->pAudioPlayThread->channelId == pThread->channelId)
				{
					DECODER_OBJ *pDecoderObj = GetDecoder(pThread, MEDIA_TYPE_AUDIO, &frameinfo);
					if (NULL == pDecoderObj)
					{
#ifdef _DEBUG
						_TRACE("[ch%d]获取音频解码器失败: %d.\n", pThread->channelId, frameinfo.codec);
#endif
						continue;
					}

					memset(audio_buf, 0x00, audbuf_len);
					int pcm_data_size = 0;
					int ret = FFD_DecodeAudio(pDecoderObj->ffDecoder, (char*)pbuf, frameinfo.length, (char *)audio_buf, &pcm_data_size);	//音频解码(支持g711(ulaw)和AAC)
					if (ret == 0)
					{
						//播放
						if (pChannelManager->pAudioPlayThread->audiochannels == 0)
						{
							pChannelManager->SetAudioParams(pDecoderObj->codec.channels, pDecoderObj->codec.samplerate, 16);//32);// 16);
						}
						if (pChannelManager->pAudioPlayThread->audiochannels != 0 && NULL!=pChannelManager->pAudioPlayThread->pSoundPlayer)
						{
							pChannelManager->pAudioPlayThread->pSoundPlayer->Write((char *)audio_buf, pcm_data_size);
						}
					}
					else
					{
#ifdef _DEBUG
						_TRACE("[ERROR]解码音频失败....\n");
#endif
					}
				}
			}
#endif
		}
		else if (MEDIA_TYPE_EVENT == mediatype)
		{
			if (frameinfo.type == 0xF1)		//Loss Packet
			{
				pThread->dwLosspacketTime	=	GetTickCount();
			}
			else if (frameinfo.type == 0xFF)	//Disconnect
			{
				pThread->dwDisconnectTime	=	GetTickCount();
			}
		}
	}

	if ( (NULL != pChannelManager) && (NULL != pChannelManager->pAudioPlayThread) && (pChannelManager->pAudioPlayThread->channelId == pThread->channelId) )
	{
		if (NULL!=pChannelManager->pAudioPlayThread->pSoundPlayer)
		{
			pChannelManager->pAudioPlayThread->pSoundPlayer->Close();
		}
		pChannelManager->pAudioPlayThread->channelId = -1;
		pChannelManager->pAudioPlayThread->audiochannels = 0;
	}


	if (NULL != pThread->mp4cHandle)
	{
		MP4C_CloseMp4File(pThread->mp4cHandle);
		MP4C_Deinit(&pThread->mp4cHandle);
		pThread->mp4cHandle = NULL;
		pThread->vidFrameNum = 0;
	}

	delete []audio_buf;
	delete []pbuf;
	pbuf = NULL;
	pThread->decodeThread.flag	=	0x00;

#ifdef _DEBUG
	_TRACE("解码线程[%d]已退出 ThreadId:%d.\n", pThread->channelId, GetCurrentThreadId());
#endif

	return 0;
}



LPTHREAD_START_ROUTINE CChannelManager::_lpDisplayThread( LPVOID _pParam )
{
	PLAY_THREAD_OBJ *pThread = (PLAY_THREAD_OBJ*)_pParam;
	if (NULL == pThread)			return 0;

	pThread->displayThread.flag	=	0x02;

#ifdef _DEBUG
	_TRACE("显示线程[%d]已启动. ThreadId:%d ...\n", pThread->channelId, GetCurrentThreadId());
#endif

	SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER)CrashHandler_Display);

	int width = 0;
	int height= 0;
	RECT	rcVideoRender;
	SetRectEmpty(&rcVideoRender);

	int iInitTimestamp = 0;

	pThread->rtpTimestamp = 0;
	unsigned int deviceLostTime = (unsigned int)time(NULL)-2;

	int iLastDisplayYuvIdx = -1;

	MEDIA_FRAME_INFO	lastFrameInfo;
	memset(&lastFrameInfo, 0x00, sizeof(MEDIA_FRAME_INFO));

//#ifdef _DEBUG
#if 1
	float	fDisplayTimes = 0.0f;		//显示耗时统计
	int		displayFrameNum = 0;
	int		nDisplayTotalTimes = 0;
	unsigned int uiLastTotalTime = 0;
#endif

	int iDelay = 0;

	_VS_BEGIN_TIME_PERIOD(1);
	QueryPerformanceFrequency(&pThread->cpuFreq);
	_VS_END_TIME_PERIOD(1);
	while (1)
	{
		if (pThread->displayThread.flag == 0x03)		break;

		int iDispalyYuvIdx = -1;
		int iYuvFrameNum = 0;
		unsigned int getNextFrame = 0;
		unsigned int rtpTimestamp = pThread->rtpTimestamp;
		do
		{
			if (pThread->displayThread.flag == 0x03)		break;

			for (int iYuvIdx=0; iYuvIdx<MAX_YUV_FRAME_NUM; iYuvIdx++)
			{
				if (pThread->displayThread.flag == 0x03)		break;
				if (pThread->yuvFrame[iYuvIdx].frameinfo.length < 1)		continue;

				if (pThread->yuvFrame[iYuvIdx].frameinfo.rtptimestamp > rtpTimestamp && getNextFrame==0)
				{
					rtpTimestamp = pThread->yuvFrame[iYuvIdx].frameinfo.rtptimestamp;
					iDispalyYuvIdx = iYuvIdx;
					getNextFrame = 1;
				}
				else if (pThread->yuvFrame[iYuvIdx].frameinfo.rtptimestamp <= rtpTimestamp)
				{
					rtpTimestamp = pThread->yuvFrame[iYuvIdx].frameinfo.rtptimestamp;
					iDispalyYuvIdx = iYuvIdx;
				}
				iYuvFrameNum ++;
			}
			_VS_BEGIN_TIME_PERIOD(1);
			__VS_Delay(1);
			_VS_END_TIME_PERIOD(1);

		}while (iDispalyYuvIdx == -1);

		pThread->dwDisconnectTime = 0;
		if (pThread->displayThread.flag == 0x03 )						break;
		if (iDispalyYuvIdx < 0 || iDispalyYuvIdx>=MAX_YUV_FRAME_NUM)	continue;

		if (pThread->yuvFrame[iDispalyYuvIdx].frameinfo.width < 1 ||
			pThread->yuvFrame[iDispalyYuvIdx].frameinfo.height< 1)
		{
			continue;
		}

		if ( (NULL==pThread->hWnd) || (NULL!=pThread->hWnd && (!IsWindow(pThread->hWnd))) || (NULL!=pThread->hWnd && (!IsWindowVisible(pThread->hWnd))))
		{
			pThread->rtpTimestamp = pThread->yuvFrame[iDispalyYuvIdx].frameinfo.rtptimestamp;

			memset(&pThread->yuvFrame[iDispalyYuvIdx].frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));
			continue;
		}

		iLastDisplayYuvIdx = iDispalyYuvIdx;

		pThread->frameQueue = 0;
		if (NULL != pThread->pAVQueue && NULL!=pThread->pAVQueue->pQueHeader)
			//if (pThread->initQueue==0x01 && NULL!=pThread->avQueue.pQueHeader)
		{
			pThread->frameQueue = pThread->pAVQueue->pQueHeader->videoframes;
		}

		int iQue1_DecodeQueue = pThread->frameQueue;		//未解码的帧数
		int iQue2_DisplayQueue= iYuvFrameNum;				//已解码的帧数

		int nQueueFrame = iQue1_DecodeQueue + iQue2_DisplayQueue;

		RECT rcSrc;
		RECT rcDst;
		SetRect(&rcSrc, 0, 0, width, height);
		if (NULL!=pThread->hWnd && (IsWindow(pThread->hWnd)) )
		{
			GetClientRect(pThread->hWnd, &rcDst);
		}

		//如果当前分辨率和之前的不同,则重新初始化d3d
		if (lastFrameInfo.width != pThread->yuvFrame[iDispalyYuvIdx].frameinfo.width ||
			lastFrameInfo.height!= pThread->yuvFrame[iDispalyYuvIdx].frameinfo.height)
		{
			pThread->resetD3d = true;
		}

		EnterCriticalSection(&pThread->crit);			//Lock

		if ( pThread->resetD3d )
		{
			pThread->resetD3d = false;
			//关闭d3d
			if (pThread->renderFormat == GDI_FORMAT_RGB24)
			{
				RGB_DeinitDraw(&pThread->d3dHandle);
			}
			else
			{
				D3D_Release(&pThread->d3dHandle);
			}
		}
		width = pThread->yuvFrame[iDispalyYuvIdx].frameinfo.width;
		height= pThread->yuvFrame[iDispalyYuvIdx].frameinfo.height;

		LeaveCriticalSection(&pThread->crit);			//Unlock

		//创建D3dRender
		if (pThread->renderFormat == GDI_FORMAT_RGB24)
		{
			if (NULL == pThread->d3dHandle)	RGB_InitDraw(&pThread->d3dHandle);
		}
		else if ( (NULL == pThread->d3dHandle) && ((unsigned int)time(NULL)-deviceLostTime >= 2) )
		{
			D3D_FONT	font;
			memset(&font, 0x00, sizeof(D3D_FONT));
			font.bold	=	0x00;
			wcscpy(font.name, TEXT("Arial Black"));
			font.size	=	(int)(float)((width)*0.02f);// 32;
			font.width	=	(int)(float)((font.size)/2.5f);//13;
			if (NULL!=pThread->hWnd && (IsWindow(pThread->hWnd)) )
			{
				D3D_Initial(&pThread->d3dHandle, pThread->hWnd, width, height, 0, 1, pThread->renderFormat, &font);
			}
			if (NULL == pThread->d3dHandle)
			{
				_TRACE("DEVICE LOST...   times:%d\n", ((unsigned int)time(NULL)-deviceLostTime));
				deviceLostTime = (unsigned int)time(NULL);
				//如果d3d 初始化失败,则清空帧头信息,以便解码线程继续解码下一帧
				pThread->rtpTimestamp = pThread->yuvFrame[iDispalyYuvIdx].frameinfo.rtptimestamp;
				memset(&pThread->yuvFrame[iDispalyYuvIdx].frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));
				_VS_BEGIN_TIME_PERIOD(1);
				__VS_Delay(1);
				_VS_END_TIME_PERIOD(1);
				continue;
			}
		}

		int fps = pThread->yuvFrame[iDispalyYuvIdx].frameinfo.fps;
		int iOneFrameUsec = 1000/30;	//normal
		if (fps>0)	iOneFrameUsec = 1000 / fps;

		int iCache = 0;
		if (NULL!=pThread->frameCache) iCache = pThread->frameCache;
	
		pThread->rtpTimestamp = pThread->yuvFrame[iDispalyYuvIdx].frameinfo.rtptimestamp;

		//统计信息:  编码格式 分辨率 帧率 帧类型  码流  缓存帧数
		char sztmp[128] = {0,};
#if 1
		sprintf(sztmp, "%s[%d x %d]  FPS: %d[%s]    Bitrate: %.2fMbps   Cache: %d / %d",
			pThread->yuvFrame[iDispalyYuvIdx].frameinfo.codec==0x1C?"H264":"MPEG4",
			pThread->yuvFrame[iDispalyYuvIdx].frameinfo.width,
			pThread->yuvFrame[iDispalyYuvIdx].frameinfo.height,
			//ipcFps,
			fps,
			pThread->yuvFrame[iDispalyYuvIdx].frameinfo.type==0x01?"I":"P",
			pThread->yuvFrame[iDispalyYuvIdx].frameinfo.bitrate/1024.0f,
			nQueueFrame, iCache);
#else
			sprintf(sztmp, "[%dx%d] fps[%d/%d] Bitrate[%.2fMbps]Cache[%d(%d+%d) / %d]  AverageTime: %.2f  Delay: %d  totaltime:%d / %d",
				pThread->yuvFrame[iDispalyYuvIdx].frameinfo.width,
				pThread->yuvFrame[iDispalyYuvIdx].frameinfo.height,
				pThread->yuvFrame[iDispalyYuvIdx].frameinfo.fps, fps,//ipcFps,
				pThread->yuvFrame[iDispalyYuvIdx].frameinfo.bitrate/1024.0f,
				nQueueFrame,  iQue1_DecodeQueue, iQue2_DisplayQueue,  iCache, 
				fDisplayTimes, iDelay, (int)fDisplayTimes+(iDelay>0?iDelay:0), iOneFrameUsec);
#endif
		D3D_OSD	osd;
		memset(&osd, 0x00, sizeof(D3D_OSD));
		MByteToWChar(sztmp, osd.string, sizeof(osd.string)/sizeof(osd.string[0]));
		if (pThread->renderFormat == GDI_FORMAT_RGB24)
		{
			SetRect(&osd.rect, 2, 2, (int)wcslen(osd.string)*7, (int)(float)((height)*0.046f));//40);
		}
		else
		{
			SetRect(&osd.rect, 2, 2, (int)wcslen(osd.string)*20, (int)(float)((height)*0.046f));//40);
		}
		osd.color = RGB(0xff,0xff,0x00);
		osd.shadowcolor = RGB(0x15,0x15,0x15);
		osd.alpha = 180;

		int showOSD = pThread->showStatisticalInfo;

		int ret = 0;

		memset(&lastFrameInfo, 0x00, sizeof(MEDIA_FRAME_INFO));
		memcpy(&lastFrameInfo, &pThread->yuvFrame[iDispalyYuvIdx].frameinfo, sizeof(MEDIA_FRAME_INFO));

		if (pThread->renderFormat == GDI_FORMAT_RGB24)
		{
			RGB_DrawData(pThread->d3dHandle, pThread->hWnd, pThread->yuvFrame[iDispalyYuvIdx].pYuvBuf, width, height, pThread->ShownToScale, RGB(0x3c,0x3c,0x3c), 0, showOSD, &osd);
			//D3D_RenderRGB24ByGDI(pThread->hWnd, pThread->yuvFrame[iDispalyYuvIdx].pYuvBuf, width, height, showOSD, &osd);
		}
		else if (NULL != pThread->d3dHandle)
		{
			D3D_UpdateData(pThread->d3dHandle, 0, (unsigned char*)pThread->yuvFrame[iDispalyYuvIdx].pYuvBuf, width, height, &rcSrc, NULL, showOSD, &osd);
			ret = D3D_Render(pThread->d3dHandle, pThread->hWnd, pThread->ShownToScale, &rcDst);
			if (ret < 0)
			{
				deviceLostTime = (unsigned int)time(NULL);
				pThread->resetD3d = true;						//需要重建D3D
			}
		}
		memset(&pThread->yuvFrame[iDispalyYuvIdx].frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));

		if (iInitTimestamp == 0x00)
		{
			iInitTimestamp = 0x01;
			_VS_BEGIN_TIME_PERIOD(1);
			__VS_Delay(50);
			QueryPerformanceCounter(&pThread->lastRenderTime);
			_VS_END_TIME_PERIOD(1);
		}
		else
		{
			unsigned int iInterval = 0;
			_LARGE_INTEGER	nowTime;

			_VS_BEGIN_TIME_PERIOD(1);
			QueryPerformanceCounter(&nowTime);
			_VS_END_TIME_PERIOD(1);
			if (pThread->cpuFreq.QuadPart < 1)	pThread->cpuFreq.QuadPart = 1;
			LONGLONG lInterval = (LONGLONG)(((nowTime.QuadPart - pThread->lastRenderTime.QuadPart) / (double)pThread->cpuFreq.QuadPart * (double)1000));
			iInterval = (int)lInterval;

			iDelay = iOneFrameUsec - iInterval;

			if (iDelay<1)	iDelay = 0;
			else if (iDelay>1500)	iDelay=1500;
			if (nQueueFrame<iCache && fps>0)
			{
				int ii = ((iOneFrameUsec * (iCache-nQueueFrame))/fps);
				iDelay += ii;
			}
			else if (nQueueFrame>iCache && fps>0)
			{
				int ii = ((iOneFrameUsec * (nQueueFrame-iCache))/fps);
				iDelay -= ii;
			}

//#ifdef _DEBUG
#if 1
			unsigned int uiCurrTime = (unsigned int)time(NULL);
			if (uiLastTotalTime == 0x00)		uiLastTotalTime = uiCurrTime;

			if (uiLastTotalTime != uiCurrTime)
			{
				fDisplayTimes = (float)nDisplayTotalTimes / (float)displayFrameNum;
				//fDisplayTimes = (float)nDisplayTotalTimes / iOneFrameUsec;
				uiLastTotalTime = uiCurrTime;
				nDisplayTotalTimes = iInterval;
				displayFrameNum = 1;
			}
			else
			{
				nDisplayTotalTimes += iInterval;
				displayFrameNum ++;
			}
#endif

			_VS_BEGIN_TIME_PERIOD(1);
			//_TRACE("[ch%d]共用时: %d\t显示耗时:%d\t延时:%d\t缓存帧数:%d\t当前帧大小:%d  OneFrameUsec:%d\n", pThread->renderCh, iInterval+iDelay, iInterval, iDelay, nQueueFrame, frame_size, iOneFrameUsec);
			if (iDelay>0 && iDelay<1500 && iCache>0 && iCache*2>nQueueFrame)
			{
				__VS_Delay(iDelay);
			}

			QueryPerformanceCounter(&pThread->lastRenderTime);
			_VS_END_TIME_PERIOD(1);
		}
	}

	if (pThread->renderFormat == GDI_FORMAT_RGB24)
	{
		RGB_DeinitDraw(&pThread->d3dHandle);
	}
	else
	{
		D3D_Release(&pThread->d3dHandle);
	}
	pThread->rtpTimestamp = 0;

	pThread->displayThread.flag	=	0x00;

#ifdef _DEBUG
	_TRACE("显示线程[%d]已退出. ThreadId:%d ..\n", pThread->channelId, GetCurrentThreadId());
#endif

	return 0;
}



int CALLBACK __NVSourceCallBack( int _chid, int *_chPtr, int _mediatype, char *pbuf, NVS_FRAME_INFO *frameinfo)
{
	PLAY_THREAD_OBJ	*pPlayThread = (PLAY_THREAD_OBJ *)_chPtr;

	if (NULL == pPlayThread)			return -1;

	if (NULL == pChannelManager)	return -1;

	if (NULL != frameinfo)
	{
		if (frameinfo->height==1088)		frameinfo->height=1080;
		else if (frameinfo->height==544)	frameinfo->height=540;
	}

	pChannelManager->ProcessData(_chid, _mediatype, pbuf, frameinfo);

	return 0;
}

int	CChannelManager::ProcessData(int _chid, int mediatype, char *pbuf, NVS_FRAME_INFO *frameinfo)
{
	if (NULL == pRealtimePlayThread)			return 0;

	if (mediatype == MEDIA_TYPE_VIDEO)
	{
		if ( (NULL == pRealtimePlayThread[_chid].pAVQueue) && (frameinfo->type==0x01/*Key frame*/) )
		{
			pRealtimePlayThread[_chid].pAVQueue = new SS_QUEUE_OBJ_T();
			if (NULL != pRealtimePlayThread[_chid].pAVQueue)
			{
				memset(pRealtimePlayThread[_chid].pAVQueue, 0x00, sizeof(SS_QUEUE_OBJ_T));
				SSQ_Init(pRealtimePlayThread[_chid].pAVQueue, 0x00, _chid, TEXT(""), MAX_AVQUEUE_SIZE, 2, 0x01);
				SSQ_Clear(pRealtimePlayThread[_chid].pAVQueue);
				pRealtimePlayThread[_chid].initQueue = 0x01;
			}

			pRealtimePlayThread[_chid].dwLosspacketTime=0;
			pRealtimePlayThread[_chid].dwDisconnectTime=0;
		}
		if (NULL != pRealtimePlayThread[_chid].pAVQueue)
		{
			SSQ_AddData(pRealtimePlayThread[_chid].pAVQueue, _chid, MEDIA_TYPE_VIDEO, (MEDIA_FRAME_INFO*)frameinfo, pbuf);
		}
	}
	else if (mediatype == MEDIA_TYPE_AUDIO)
	{
		if (NULL != pRealtimePlayThread[_chid].pAVQueue)
		{
			SSQ_AddData(pRealtimePlayThread[_chid].pAVQueue, _chid, MEDIA_TYPE_AUDIO, (MEDIA_FRAME_INFO*)frameinfo, pbuf);
		}
	}
	else if (mediatype == MEDIA_TYPE_EVENT)
	{
		if (NULL == pbuf && NULL == frameinfo)
		{
			_TRACE("[ch%d]连接中...\n", _chid);

			MEDIA_FRAME_INFO	frameinfo;
			memset(&frameinfo, 0x00, sizeof(MEDIA_FRAME_INFO));
			frameinfo.length = 1;
			frameinfo.type   = 0xFF;
			SSQ_AddData(pRealtimePlayThread[_chid].pAVQueue, _chid, MEDIA_TYPE_EVENT, (MEDIA_FRAME_INFO*)&frameinfo, "1");
		}
		else if (NULL!=frameinfo && frameinfo->type==0xF1)
		{
			_TRACE("[ch%d]掉包[%.2f]...\n", _chid, frameinfo->losspacket);

			frameinfo->length = 1;
			SSQ_AddData(pRealtimePlayThread[_chid].pAVQueue, _chid, MEDIA_TYPE_EVENT, (MEDIA_FRAME_INFO*)frameinfo, "1");
		}
	}

	return 0;
}


int		CChannelManager::StartManuRecording(int channelId)
{
	if (NULL == pRealtimePlayThread)			return -1;

	int iNvsIdx = channelId - CHANNEL_ID_GAIN;
	if (iNvsIdx < 0 || iNvsIdx>= MAX_CHANNEL_NUM)	return -1;

	if (NULL == pRealtimePlayThread[iNvsIdx].mp4cHandle)
	{
		pRealtimePlayThread[iNvsIdx].manuRecording = 0x01;
	}

	return 0;
}
int		CChannelManager::StopManuRecording(int channelId)
{
	if (NULL == pRealtimePlayThread)			return -1;

	int iNvsIdx = channelId - CHANNEL_ID_GAIN;
	if (iNvsIdx < 0 || iNvsIdx>= MAX_CHANNEL_NUM)	return -1;

	if (pRealtimePlayThread[iNvsIdx].manuRecording == 0x01)
	{
		pRealtimePlayThread[iNvsIdx].manuRecording = 0x00;
	}

	return 0;
}
