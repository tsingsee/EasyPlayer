
// Intel Media Hardware Codec SDK Interface [8/17/2016 SwordTwelve]

#ifndef INTELHARDCODEC_INTERFACE_H
#define INTELHARDCODEC_INTERFACE_H

#ifdef __cplusplus


class IntelHardCodec_Interface
{
	//导出接口
public:
public:	//DLL 接口
	virtual int  WINAPI Init(HWND hWnd,int mode = 1) = 0;
	virtual int  WINAPI Decode(unsigned char * pData, int len, OUT unsigned char * pYUVData) = 0;
	virtual void WINAPI	Close() = 0;

};

//视频获取定义接口指针类型
typedef	IntelHardCodec_Interface*	LPIntelHardDecoder;	

LPIntelHardDecoder	APIENTRY Create_IntelHardDecoder();//创建控制接口指针
void APIENTRY Release_IntelHardDecoder(LPIntelHardDecoder lpHardDecoder);//销毁控制接口指针

#endif//__cplusplus
#endif//INTELHARDCODEC_INTERFACE_H