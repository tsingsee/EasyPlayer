/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
	Author: Gavin@easydarwin.org
*/
#include "libEasyPlayerAPI.h"
#include "ChannelManager.h"
CChannelManager	*g_pChannelManager = NULL;


// ³õÊ¼»¯SDK
LIB_EASYPLAYER_API int EasyPlayer_Init()
{
	if (NULL == g_pChannelManager)
	{
		g_pChannelManager = new CChannelManager();
		g_pChannelManager->Initial();
	}

	if (NULL == g_pChannelManager)		return -1;

	return 0;
}

// Release
LIB_EASYPLAYER_API void EasyPlayer_Release()
{
	if (NULL != g_pChannelManager)
	{
		delete g_pChannelManager;
		g_pChannelManager = NULL;
	}
}


LIB_EASYPLAYER_API int EasyPlayer_OpenStream(const char *url, HWND hWnd, RENDER_FORMAT renderFormat, int rtpovertcp,const char *username, const char *password, MediaSourceCallBack callback, void *userPtr)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->OpenStream(url, hWnd, renderFormat, rtpovertcp, username, password, callback, userPtr);
}

LIB_EASYPLAYER_API void EasyPlayer_CloseStream(int channelId)
{
	if (NULL == g_pChannelManager)		return;

	g_pChannelManager->CloseStream(channelId);
}

LIB_EASYPLAYER_API int EasyPlayer_SetFrameCache(int channelId, int cache)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->SetFrameCache(channelId, cache);
}
LIB_EASYPLAYER_API int EasyPlayer_SetShownToScale(int channelId, int shownToScale)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->SetShownToScale(channelId, shownToScale);
}

LIB_EASYPLAYER_API int EasyPlayer_SetDecodeType(int channelId, int decodeKeyframeOnly)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->SetDecodeType(channelId, decodeKeyframeOnly);
}
LIB_EASYPLAYER_API int EasyPlayer_SetRenderRect(int channelId, LPRECT lpSrcRect)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->SetRenderRect(channelId, lpSrcRect);
}

LIB_EASYPLAYER_API int EasyPlayer_ShowStatisticalInfo(int channelId, int show)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->ShowStatisticalInfo(channelId, show);
}

LIB_EASYPLAYER_API int EasyPlayer_SetDragStartPoint(int channelId, POINT pt)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->SetDragStartPoint(channelId, pt);
}
LIB_EASYPLAYER_API int EasyPlayer_SetDragEndPoint(int channelId, POINT pt)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->SetDragEndPoint(channelId, pt);
}
LIB_EASYPLAYER_API int EasyPlayer_ResetDragPoint(int channelId)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->ResetDragPoint(channelId);
}


LIB_EASYPLAYER_API int EasyPlayer_PlaySound(int channelId)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->PlaySound(channelId);
}
LIB_EASYPLAYER_API int EasyPlayer_StopSound()
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->StopSound();
}

LIB_EASYPLAYER_API int EasyPlayer_StartManuRecording(int channelId)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->StartManuRecording(channelId);
}

LIB_EASYPLAYER_API int EasyPlayer_StopManuRecording(int channelId)
{
	if (NULL == g_pChannelManager)		return -1;

	return g_pChannelManager->StopManuRecording(channelId);
}

