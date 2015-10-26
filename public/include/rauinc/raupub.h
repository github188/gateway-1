#ifndef _RAUPUB_H

#define _RAUPUB_H


#ifdef RAUPUB_EXPORTS

#define RAUPUB_API __declspec(dllexport)

#else

#ifdef WIN32

#define RAUPUB_API __declspec(dllimport)

#else

#define RAUPUB_API 

#endif

#endif


#ifdef __cplusplus

extern "C" {

#endif



#ifndef ENDFIELD

#define ENDFIELD -1

#endif


typedef struct
{
	unsigned char buf[48];

} SYSHEAD;



typedef struct
{
	unsigned short SourceNo;
	SYSHEAD syshead;  

} TRUSERID;



typedef struct
{
	unsigned short SourceNo;
	unsigned short DestNo;
	unsigned short FuncNo;
	unsigned int   BatchNo;

} TPUSHDEST;


typedef struct
{
   char szSvrIP[20];
   int  iSvrPort;
   int  iHBInterval;
   char  szRAUGroupID[12];
   void *pSvrLink;

} ST_RAUNIT;

extern ST_RAUNIT g_RAUnit;

RAUPUB_API int SetCol(TRUSERID *handle,...);

RAUPUB_API int PutRowData(ST_PACK *pdata);

int AnswerData(TRUSERID *ruserid,int retcode,char *szmsg);

RAUPUB_API int PutRow(TRUSERID *handle,ST_PACK *pdata,int *pRetCode,char *rtnMsg);

#define ADP_ROWS  50

RAUPUB_API int AnswerDataPart(TRUSERID *ruserid,int retcode,char *szmsg);

RAUPUB_API int AnswerDataEx(TRUSERID *ruserid,ST_CPACK *apack,ST_PACK *pArrays,int retcode,char *szmsg);

RAUPUB_API int PushData(int sourceno,int destno,int funcno,int batchno,ST_CPACK *ppack,char pushmode,int acktime);

RAUPUB_API int PushDataEx(TPUSHDEST *pushdests,int pdcount,ST_CPACK *ppack,char pushmode,int acktime);

RAUPUB_API int ExtCall(int sourceno,int destno,int funcno,int batchno,int acktime,ST_CPACK *rpack,ST_CPACK *apack,ST_PACK *pArrays);

RAUPUB_API int ExtCallNext(int acktime,ST_CPACK *apack,ST_PACK *pArrays);

RAUPUB_API void ResetNormalCPack(ST_CPACK *pack,unsigned char nextflag,int recCount);

RAUPUB_API int CheckProcCancel();

RAUPUB_API int CT_ERR_DEAL(char *msg,int retcode);

RAUPUB_API int Error_Deal(int retcode,char *omsg,char *pFormat,...);

RAUPUB_API void SQLDebugLog(int errorcode,char *pFormat,...);

RAUPUB_API void TextLogOut(int errorcode, char *textlog);

#ifndef AM_AUTO

#define AM_AUTO     'A'
#define AM_CONFIRM  'C'

#endif

extern char iDefaultAnswerMode;   // 本系统缺省的应答模式

RAUPUB_API int SetAnswerMode(char iAnswerMode);

RAUPUB_API int RACMsgLogOut(int logid,char *pFormat,...);

RAUPUB_API int SetMaxRow(int maxrows);

RAUPUB_API int SetRAUError(int errcode,char *pFormat,...);

RAUPUB_API int SetAnswerTimeout(unsigned int atimeout);

RAUPUB_API int ResetAnswerData();


#ifdef __cplusplus

}

#endif



#undef RAUPUB_API


#endif

