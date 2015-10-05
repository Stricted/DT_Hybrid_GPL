#ifndef __ATP_DLNAAPI_H__
#define __ATP_DLNAAPI_H__

#include "mdbapi.h"
#include "avformat.h"
#include "avcodec.h"

/* 用于支持LPCM格式以通过DLNA认证 */
#define SUPPORT_ATP_DMS_LPCM  1

/*
 *  ProfileIdx + ClassID + Restricted + ObjectType
 *    16 bits         8 bits         1 bits          7 bits
 */

#define ATP_DLNA_DEF_PROFILE_ID(objType, restricted, classId)   ((((VOS_UINT32)(objType)) & 0x0000007F) + ((((VOS_UINT32)(restricted)) & 0x00000001) << 7) + ((((VOS_UINT32)(classId)) & 0x000000FF) << 8))

#define ATP_DLNA_GET_PROFILE_IDX(id)          ((((VOS_UINT32)(id)) & 0xFFFF0000) >> 16)
#define ATP_DLNA_GET_CLASS_ID(id)             ((((VOS_UINT32)(id)) & 0x0000FF00) >> 8)
#define ATP_DLNA_GET_OBJECT_ID(id)            (((VOS_UINT32)(id)) & 0x0000007F)

#define ATP_DLNA_FLAG_AUDIO_VIDEO		"01700000000000000000000000000000"
#define ATP_DLNA_FLAG_VIDEO_TIMESEEK	"01700000000000000000000000000000"
#define ATP_DLNA_FLAG_PHOTO		        "00F00000000000000000000000000000"

typedef enum tagATP_DLNA_PROFILE_CLASS_EN
{
    // Image Class
    ATP_DLNA_PROFILE_IMAGE_JPEG,
    ATP_DLNA_PROFILE_IMAGE_PNG,
    ATP_DLNA_PROFILE_IMAGE_GIF,
    // Audio Class
    ATP_DLNA_PROFILE_AUDIO_AC3,
    ATP_DLNA_PROFILE_AUDIO_AMR,
    ATP_DLNA_PROFILE_AUDIO_ATRAC3,
    ATP_DLNA_PROFILE_AV_WAV,
    ATP_DLNA_PROFILE_AUDIO_LPCM,
    ATP_DLNA_PROFILE_AUDIO_MP3,
    ATP_DLNA_PROFILE_AUDIO_MPEG4,
    ATP_DLNA_PROFILE_AUDIO_WMA,
    // AV Class
    ATP_DLNA_PROFILE_AV_MPEG1,
    ATP_DLNA_PROFILE_AV_MPEG2,
    ATP_DLNA_PROFILE_AV_MPEG4_PART2,
    ATP_DLNA_PROFILE_AV_MPEG4_PART10, // a.k.a. MPEG-4 AVC
    ATP_DLNA_PROFILE_AV_WMV9,
    ATP_DLNA_PROFILE_AV_WMV,    
    ATP_DLNA_PROFILE_AV_AVI,
    ATP_DLNA_PROFILE_AV_MKV,
    ATP_DLNA_PROFILE_AV_RM,
    ATP_DLNA_PROFILE_AV_MPG,
    ATP_DLNA_PROFILE_AV_MPEG4,
    ATP_DLNA_PROFILE_AV_TS,
    ATP_DLNA_PROFILE_AUDIO_MPEG_4
    // TODO: Add new profile classes here
} ATP_DLNA_PROFILE_CLASS_EN;

typedef struct tagATP_DLNA_PROFILES_ST
{
    // Profile ID, part of DLNA.ORG_PN= string
    const VOS_CHAR      *id;
    // Profile MIME type
    const VOS_CHAR      *mime;
    // Profile Label
    const VOS_CHAR      *label;
    // Profile type: IMAGE / AUDIO / AV, ATP_MDB_OBJECT_CLASS_EN
    //VOS_UINT32          ulClass;
    VOS_INT8            anounce;
} ATP_DLNA_PROFILES_ST;

typedef struct tagATP_DLNA_CODEC_INFO_ST
{
    // audio stream and codec
    AVStream        *as;
    AVCodecContext  *ac;

    // video stream and codec
    AVStream        *vs;
    AVCodecContext  *vc;
} ATP_DLNA_CODEC_INFO_ST;

typedef enum tagATP_DLNA_CONTAINER_TYPE_EN
{
  ATP_DLNA_CT_UNKNOWN,
  ATP_DLNA_CT_IMAGE,        // for PNG and JPEG
  ATP_DLNA_CT_ASF,          // usually for WMA/WMV
  ATP_DLNA_CT_AMR,
  ATP_DLNA_CT_AAC,
  ATP_DLNA_CT_AC3,
  ATP_DLNA_CT_MP3,
  ATP_DLNA_CT_WAV,
  ATP_DLNA_CT_MOV,
  ATP_DLNA_CT_3GP,
  ATP_DLNA_CT_MP4,
  ATP_DLNA_CT_FF_MPEG,      // FFMPEG "mpeg"
  ATP_DLNA_CT_FF_MPEG_TS,   // FFMPEG "mpegts"
  ATP_DLNA_CT_MPEG_ELEMENTARY_STREAM,
  ATP_DLNA_CT_MPEG_PROGRAM_STREAM,
  ATP_DLNA_CT_MPEG_TRANSPORT_STREAM,
  ATP_DLNA_CT_MPEG_TRANSPORT_STREAM_DLNA,
  ATP_DLNA_CT_MPEG_TRANSPORT_STREAM_DLNA_NO_TS,
} ATP_DLNA_CONTAINER_TYPE_EN;

ATP_DLNA_CONTAINER_TYPE_EN ATP_DLNA_GetStreamContainerType(const AVFormatContext *ctx);
const ATP_DLNA_PROFILES_ST *ATP_DLNA_GetProfileByProfileId(VOS_UINT32 ulProfileId);

typedef struct tagATP_DLNA_MEDIA_CLASS_ST
{
    const VOS_CHAR          *pcExtensions;
    const ATP_DLNA_PROFILES_ST *(*pfProbe)(const AVFormatContext *pstFormatCtx,
                         ATP_DLNA_CONTAINER_TYPE_EN enType,
                         const ATP_DLNA_CODEC_INFO_ST *pstCodecs,
                         VOS_UINT32 *pulIdx);
    const ATP_DLNA_PROFILES_ST *(*pfGetProfileByIdx)(VOS_UINT32 ulIdx);
    const VOS_CHAR          *pcDLNAOP;
    const VOS_CHAR          *pcDLNAFLAGS;
} ATP_DLNA_MEDIA_CLASS_ST;

typedef struct tagATP_DLNA_CLASS_PAIR_ST
{
    VOS_UINT32                      ulClassId;
    const ATP_DLNA_MEDIA_CLASS_ST   *pstMediaClass;
} ATP_DLNA_CLASS_PAIR_ST;

/*
 *  Public utilities
 */
extern const ATP_DLNA_CLASS_PAIR_ST g_astMediaClasses[];
extern const VOS_UINT32  g_ulNumOfMediaClasses;

VOS_BOOL ATP_DLNA_MatchByExtension(const VOS_CHAR *pcFileName);
VOS_BOOL ATP_DLNA_ProbeFile(
                        const AVFormatContext           *ctx,
                        ATP_DLNA_CONTAINER_TYPE_EN      enContainerType,
                        const ATP_DLNA_CODEC_INFO_ST    *pstCodecInfo,
                        const ATP_DLNA_PROFILES_ST      **ppstProfile,
                        VOS_UINT32                      *pulProfileId);
VOS_BOOL ATP_DLNA_GetCodecsInfo(const AVFormatContext *ctx, ATP_DLNA_CODEC_INFO_ST *pstCodecInfo);

VOS_BOOL ATP_DLNA_IsStreamImage(const AVFormatContext           *ctx,
                                        const ATP_DLNA_CODEC_INFO_ST    *codecs,
                                        ATP_DLNA_CONTAINER_TYPE_EN      st);

VOS_BOOL ATP_DLNA_IsStreamAudio(const ATP_DLNA_CODEC_INFO_ST *codecs);

VOS_BOOL ATP_DLNA_IsStreamAV(const ATP_DLNA_CODEC_INFO_ST *codecs);

#ifdef SUPPORT_ATP_DMS_LPCM
VOS_BOOL  ATP_DLNA_MatchByLpcm(const VOS_CHAR *pcFileName);
VOS_INT32 ATP_DLNA_BuildCtxByLpcm(AVFormatContext **ppstFormatCtx, const VOS_CHAR *pszFileName);
VOS_BOOL  ATP_DLNA_ProbeFileByLpcm(const VOS_CHAR *pszFileName, VOS_UINT32 *pulProfileId);
VOS_INT32 ATP_DLNA_FreeCtxByLpcm(AVFormatContext **ppstFormatCtx);
#endif

#endif

