/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// Add by SwordTwelve
// EasyMP4Writer.h: interface for the ZMP4AVC1class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ZMP4GPAC_H)
#define ZMP4GPAC_H

#include <time.h>

#include <gpac/isomedia.h>
#pragma comment(lib,"libgpac.lib")

#ifndef ZOUTFILE_FLAG_FULL
#define ZOUTFILE_FLAG_VIDEO 0x01
#define ZOUTFILE_FLAG_AUDIO 0x02
#define ZOUTFILE_FLAG_FULL (ZOUTFILE_FLAG_AUDIO|ZOUTFILE_FLAG_VIDEO)
#endif

#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))


class EasyMP4Writer 
{
	//设置风格
	int		m_flag;//包含音频视频

	long	m_audiostartimestamp;
	long	m_videostartimestamp;

	GF_ISOFile *p_file;//MP4文件
    GF_AVCConfig *p_config;//MP4配置
    GF_ISOSample *p_videosample;//MP4帧
	GF_ISOSample *p_audiosample;//MP4帧

	
	int m_wight;
	int m_hight;
	int m_videtrackid;
	int m_audiotrackid;

	unsigned int i_videodescidx;
	unsigned int i_audiodescidx;
	bool m_bwritevideoinfo;
	bool m_bwriteaudioinfo;
	unsigned char* m_psps;
	unsigned char* m_ppps;
	int m_spslen;
	int m_ppslen;

	int m_nCreateFileFlag;

public:
	EasyMP4Writer();
	virtual ~EasyMP4Writer();
public:

	bool CreateMP4File(char*filename,int flag);
	//sps,pps第一个字节为0x67或68,
	bool WriteH264SPSandPPS(unsigned char*sps,int spslen,unsigned char*pps,int ppslen,int width,int height);
	//写入AAC信息
	bool WriteAACInfo(unsigned char*info,int len, int nSampleRate, int nChannel, int nBitsPerSample);
	//写入一帧，前四字节为该帧NAL长度
	bool WriteH264Frame(unsigned char*data,int len,bool keyframe,long timestamp);
	//写入aac数据，只有raw数据
	bool WriteAACFrame(unsigned char*data,int len,long timestamp);
	int WriteAACToMp4File(unsigned char*data,int len,long timestamp, int sample_rate, int channels, int bits_per_sample);

	//保存文件
	bool SaveFile();
	
	bool CanWrite();
	int WriteMp4File(unsigned char* pdata, int datasize, bool keyframe, long nTimestamp, int nWidth, int nHeight);

private:
	unsigned char* FindNal(unsigned char*buff,int inlen,int&outlen,bool&end);
	int find_nal_unit(unsigned char* buf, int size, int* nal_start, int* nal_end);

};

#endif // !defined(AFX_ZMP4_H__CD8C3DF9_A2A4_494D_948E_5672ADBE739D__INCLUDED_)


#if 0

/*!
 * \brief Scene ObjectTypeIndication Formats
 *
 *	Supported ObjectTypeIndication for scene description streams. *_FILE_* are only used with private scene streams
 * and only carry the file name for the scene. Other internal stream types can be used in a real streaming environment
*/
enum
{
	/*!OTI for BIFS v1*/
	GPAC_OTI_SCENE_BIFS = 0x01,
	/*!OTI for OD v1*/
	GPAC_OTI_OD_V1 = 0x01,
	/*!OTI for BIFS v2*/
	GPAC_OTI_SCENE_BIFS_V2 = 0x02,
	/*!OTI for OD v2*/
	GPAC_OTI_OD_V2 = 0x02,
	/*!OTI for BIFS InputSensor streams*/
	GPAC_OTI_SCENE_INTERACT = 0x03,
	/*!OTI for streams with extended BIFS config*/
	GPAC_OTI_SCENE_BIFS_EXTENDED = 0x04,
	/*!OTI for AFX streams with AFXConfig*/
	GPAC_OTI_SCENE_AFX = 0x05,
	/*!OTI for Font data streams */
	GPAC_OTI_FONT = 0x06,
	/*!OTI for synthesized texture streams */
	GPAC_OTI_SCENE_SYNTHESIZED_TEXTURE = 0x07,
	/*!OTI for streaming text streams */
	GPAC_OTI_TEXT_MPEG4 = 0x08,
	/*!OTI for LASeR streams*/
	GPAC_OTI_SCENE_LASER = 0x09,
	/*!OTI for SAF streams*/
	GPAC_OTI_SCENE_SAF = 0x0A,

	/*!OTI for MPEG-4 Video Part 2 streams*/
	GPAC_OTI_VIDEO_MPEG4_PART2 = 0x20,
	/*!OTI for MPEG-4 Video Part 10 (H.264 | AVC ) streams*/
	GPAC_OTI_VIDEO_AVC = 0x21,
	/*!OTI for AVC Parameter sets streams*/
	GPAC_OTI_VIDEO_AVC_PS = 0x22,
	/*!OTI for HEVC video */
	GPAC_OTI_VIDEO_HEVC = 0x23,
	/*!OTI for H264-SVC streams*/
	GPAC_OTI_VIDEO_SVC = 0x24,
	/*!OTI for HEVC layered streams*/
	GPAC_OTI_VIDEO_LHVC = 0x25,
	/*!OTI for MPEG-4 AAC streams*/
	GPAC_OTI_AUDIO_AAC_MPEG4 = 0x40,

	/*!OTI for MPEG-2 Visual Simple Profile streams*/
	GPAC_OTI_VIDEO_MPEG2_SIMPLE = 0x60,
	/*!OTI for MPEG-2 Visual Main Profile streams*/
	GPAC_OTI_VIDEO_MPEG2_MAIN = 0x61,
	/*!OTI for MPEG-2 Visual SNR Profile streams*/
	GPAC_OTI_VIDEO_MPEG2_SNR = 0x62,
	/*!OTI for MPEG-2 Visual SNR Profile streams*/
	GPAC_OTI_VIDEO_MPEG2_SPATIAL = 0x63,
	/*!OTI for MPEG-2 Visual SNR Profile streams*/
	GPAC_OTI_VIDEO_MPEG2_HIGH = 0x64,
	/*!OTI for MPEG-2 Visual SNR Profile streams*/
	GPAC_OTI_VIDEO_MPEG2_422 = 0x65,


	/*!OTI for MPEG-2 AAC Main Profile streams*/
	GPAC_OTI_AUDIO_AAC_MPEG2_MP = 0x66,
	/*!OTI for MPEG-2 AAC Low Complexity Profile streams*/
	GPAC_OTI_AUDIO_AAC_MPEG2_LCP = 0x67,
	/*!OTI for MPEG-2 AAC Scalable Sampling Rate Profile streams*/
	GPAC_OTI_AUDIO_AAC_MPEG2_SSRP = 0x68,
	/*!OTI for MPEG-2 Audio Part 3 streams*/
	GPAC_OTI_AUDIO_MPEG2_PART3 = 0x69,
	/*!OTI for MPEG-1 Video streams*/
	GPAC_OTI_VIDEO_MPEG1 = 0x6A,
	/*!OTI for MPEG-1 Audio streams*/
	GPAC_OTI_AUDIO_MPEG1 = 0x6B,
	/*!OTI for JPEG streams*/
	GPAC_OTI_IMAGE_JPEG = 0x6C,
	/*!OTI for PNG streams*/
	GPAC_OTI_IMAGE_PNG = 0x6D,
	/*!OTI for JPEG-2000 streams*/
	GPAC_OTI_IMAGE_JPEG_2000 = 0x6E,

	/*!
	 * \brief Extra ObjectTypeIndication
	 *
	 *	ObjectTypeIndication for media (audio/video) codecs not defined in MPEG-4. Since GPAC signals streams through MPEG-4 Descriptions,
	 *	it needs extensions for non-MPEG-4 streams such as AMR, H263 , etc.\n
	 *\note The decoder specific info for such streams is always carried encoded, with the following syntax:\n
	 *	DSI Syntax for audio streams
	 \code
	 *	u32 codec_four_cc: the codec 4CC reg code / codec id for ffmpeg
	 *	u32 sample_rate: sampling rate or 0 if unknown
	 *	u16 nb_channels: num channels or 0 if unknown
	 *	u16 frame_size: num audio samples per frame or 0 if unknown
	 *	u8 nb_bits_per_sample: nb bits or 0 if unknown
	 *	u8 num_frames_per_au: num audio frames per AU (used in 3GPP, max 15), 0 if unknown
	 *	char *data: per-codec extensions till end of DSI bitstream
	 \endcode
	 \n
	 *	DSI Syntax for video streams
	 \code
	 *	u32 codec_four_cc: the codec 4CC reg code  / codec id for ffmpeg
	 *	u16 width: video width or 0 if unknown
	 *	u16 height: video height or 0 if unknown
	 *	char *data: per-codec extensions till end of DSI bitstream
	 \endcode
	*/
	GPAC_OTI_MEDIA_GENERIC = 0x80,
	/*!
	 * \brief FFMPEG ObjectTypeIndication
	 *
	 *	ObjectTypeIndication for FFMPEG codecs not defined in MPEG-4. FFMPEG uses the base GPAC_OTI_MEDIA_GENERIC specific info formats, and extends it as follows:
	 \code
	 *	u32 bit_rate: the stream rate or 0 if unknown
	 *	u32 codec_tag: FFMPEG codec tag as defined in libavcodec
	 *	char *data: codec extensions till end of DSI bitstream
	 \endcode
	 */
	GPAC_OTI_MEDIA_FFMPEG = 0x81,

	/*!OTI for EVRC Voice streams*/
	GPAC_OTI_AUDIO_EVRC_VOICE = 0xA0,
	/*!OTI for SMV Voice streams*/
	GPAC_OTI_AUDIO_SMV_VOICE = 0xA1,
	/*!OTI for 3GPP2 CMF streams*/
	GPAC_OTI_3GPP2_CMF = 0xA2,
	/*!OTI for SMPTE VC-1 Video streams*/
	GPAC_OTI_VIDEO_SMPTE_VC1 = 0xA3,
	/*!OTI for Dirac Video streams*/
	GPAC_OTI_VIDEO_DIRAC = 0xA4,
	/*!OTI for AC-3 audio streams*/
	GPAC_OTI_AUDIO_AC3 = 0xA5,
	/*!OTI for enhanced AC-3 audio streams*/
	GPAC_OTI_AUDIO_EAC3 = 0xA6,
	/*!OTI for DRA audio streams*/
	GPAC_OTI_AUDIO_DRA = 0xA7,
	/*!OTI for ITU G719 audio streams*/
	GPAC_OTI_AUDIO_ITU_G719 = 0xA8,
	/*!OTI for DTS Coherent Acoustics audio streams*/
	GPAC_OTI_AUDIO_DTS_CA = 0xA9,
	/*!OTI for DTS-HD High Resolution audio streams*/
	GPAC_OTI_AUDIO_DTS_HD_HR = 0xAA,
	/*!OTI for DTS-HD Master audio streams*/
	GPAC_OTI_AUDIO_DTS_HD_MASTER = 0xAB,

	/*!OTI for dummy streams (dsi = file name) using the generic context loader (BIFS/VRML/SWF/...) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_GENERIC = 0xC0,
	/*!OTI for SVG dummy stream (dsi = file name) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_SVG = 0xC1,
	/*!OTI for LASeR/SAF+XML dummy stream (dsi = file name) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_LASER = 0xC2,
	/*!OTI for XBL dummy streams (dsi = file name) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_XBL = 0xC3,
	/*!OTI for EPG dummy streams (dsi = null) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_EPG = 0xC4,
	/*!OTI for WGT dummy streams (dsi = null) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_WGT = 0xC5,
	/*!OTI for VTT dummy stream (dsi = file name) - GPAC internal*/
	GPAC_OTI_PRIVATE_SCENE_VTT = 0xC6,

	/*!OTI for streaming SVG - GPAC internal*/
	GPAC_OTI_SCENE_SVG = 0xD0,
	/*!OTI for streaming SVG + gz - GPAC internal*/
	GPAC_OTI_SCENE_SVG_GZ = 0xD1,
	/*!OTI for DIMS (dsi = 3GPP DIMS configuration) - GPAC internal*/
	GPAC_OTI_SCENE_DIMS = 0xD2,
	/*!OTI for streaming VTT - GPAC internal*/
	GPAC_OTI_SCENE_VTT = 0xD3,
	/*!OTI for streaming VTT from MP4- GPAC internal*/
	GPAC_OTI_SCENE_VTT_MP4 = 0xD4,
	/*!OTI for streaming simple text from MP4- GPAC internal*/
	GPAC_OTI_SCENE_SIMPLE_TEXT_MP4 = 0xD5,

	/*!
	 * \brief OGG ObjectTypeIndication
	 *
	 *	Object type indication for all OGG media. The DSI contains all intitialization ogg packets for the codec
	 * and is formated as follows:\n
	 *\code
		while (dsi_size) {
			bit(16) packet_size;
			char packet[packet_size];
			dsi_size -= packet_size;
		}\endcode
	*/
	GPAC_OTI_MEDIA_OGG = 0xDD,
	GPAC_OTI_MEDIA_THEORA = 0xDF,

	GPAC_OTI_MEDIA_SUBPIC = 0xE0,

	/*!OTI for 13K Voice / QCELP audio streams*/
	GPAC_OTI_AUDIO_13K_VOICE = 0xE1,

	/*!OTI for RAW media streams. Input data is directly dispatched to the composition memory.  The DSI contains is formated (MSBF) as follows:\n
	*	DSI Syntax for audio streams
	\code
	*	u32 sample_rate: sampling rate
	*	u16 nb_channels: num channels
	*	u16 nb_bits_per_sample: num of bits per audio sample
	*	u32 frame_size: max size of audio frame in byte
	*	u32 channel_config: GPAC mask of GF_AUDIO_CH_ constants, or 0 if unknown
	\endcode
	\n
	*	DSI Syntax for video streams
	\code
	*	u32 codec_four_cc: the codec 4CC reg code  / codec id for ffmpeg
	*	u16 width: video width or 0 if unknown
	*	u16 height: video height or 0 if unknown
	*	u32 frame_size: size of the video frame
	*	u32 stride: horizontal stride of the video frame
	\endcode
	*/
	GPAC_OTI_RAW_MEDIA_STREAM = 0x101,

	/*!OTI for LIBPLAYER private streams. The data pointer in the DSI is the libplayer handle object*/
	GPAC_OTI_PRIVATE_MEDIA_LIBPLAYER = 0xF1
};

#endif
