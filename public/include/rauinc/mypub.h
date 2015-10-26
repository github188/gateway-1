#ifndef __MYPUB_H_

#define __MYPUB_H_


#ifdef RAUPUB_EXPORTS

#define RAUPUB_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define RAUPUB_API __declspec(dllimport)

#else

#define RAUPUB_API 

#endif

#endif


#ifdef __cplusplus

extern "C" {

#endif


#ifndef BYTE

typedef unsigned char	BYTE;

typedef unsigned short	WORD;

#endif

#ifndef INT4

typedef char			INT1;

typedef short			INT2;

typedef int				INT4;

#endif

#ifndef REAL4

typedef float			REAL4;

typedef double			REAL8;

#endif


//在Windows下，重新定义MAX和MIN宏

#ifdef WIN32

#define MAX(a,b) _MAX(a,b)

#define MIN(a,b) _MIN(a,b)

#endif


#ifndef ASSERT

#ifdef _DEBUG

#define ASSERT(booleanExpression )\
	if(!(booleanExpression))\
	{\
		RAISE_DESIGN_ERROR(#booleanExpression);\
	}



#define DebugMessageDisplay(msg)   printf("%s:%d <%s>\n",__FILE__,__LINE__,msg)



#else

#define ASSERT(booleanExpression)

#define DebugMessageDisplay(msg)

#endif

#endif



#include <fcntl.h>

#include <sys/stat.h>

#ifdef WIN32

#include <share.h>

#define sh_fopen(a,b,c) _fsopen(a,b,c)

#include <io.h>

#define sh_open(a,b,c)  _sopen(a,b|O_BINARY,c,S_IREAD | S_IWRITE)

#else

#define SH_DENYNO    0x40

#define SH_DENYWR    0x20

#define sh_fopen(a,b,c) fopen(a,b)

#define sh_open(a,b,c)  open(a,b,S_IREAD | S_IWRITE)

#endif





/* 去除左右空格   */

RAUPUB_API char *mytrim(char *s);


RAUPUB_API char *Strncpy(char *dest,char *source,unsigned int count);


RAUPUB_API char *Strncpy_t(char *dest,char *source,unsigned int count);


RAUPUB_API char *mytrim_r(char *dest);


RAUPUB_API char *Strncpy_rt(char *dest,char *source,unsigned int count);

RAUPUB_API int percolate(char *str);

RAUPUB_API char *getfmttime(void *ptm,int fmttype,char *outstr);


RAUPUB_API void mysleep(unsigned int milliseconds);



#ifndef WIN32

RAUPUB_API unsigned int GetTickCount();

#endif



RAUPUB_API unsigned int tc2_tc1(unsigned int tc2,unsigned int tc1);



RAUPUB_API void DataDumpFile(char *filename,char *data,int datalen);


RAUPUB_API void OutErrorFile(const char *efilename, const char *msg, const char *sourcefile, int lineno);


RAUPUB_API void DebugFileOut(char *dfilename,char *msg,char *sourcefile,int lineno);

RAUPUB_API int RemoveOverdueFile(char *path_mark,char *ext,int overdue);





//抛出一个CDesignError异常

//@参数	msg	char*	指定的错误信息

//@异常	CDesignError	抛出此异常，错误位置在当前文件当前行

#define RAISE_DESIGN_ERROR(msg) { OutErrorFile("DesignError",msg,__FILE__,__LINE__); exit(-1000);}



//抛出一个CRuntimeError

//@参数	msg	char*	指定的错误信息

//@异常	CRuntimeError	抛出此异常，错误位置在当前文件当前行

#define RAISE_RUNTIME_ERROR(msg) { OutErrorFile("RuntimeError",msg,__FILE__,__LINE__); exit(-1001);}


extern int iThisProcID;   // 本程序的ID标识，由应用服务架构分配，可以认为在整个系统中唯一，如用OutErrorFile作为文件名记录跟踪数据用 


#if defined(RAUPUB_EXPORTS)||defined(RAUPUBLIB)

#define DEBUG_RUNTIME_MSGOUT(msg) { g_pLogFile->WriteLogEx(1111,"FILE:%s LINE:%d Msg:%s",__FILE__,__LINE__,msg); }

#else

#define DEBUG_RUNTIME_MSGOUT(msg) { g_LogFile.WriteLogEx(1111,"FILE:%s LINE:%d Msg:%s",__FILE__,__LINE__,msg); }

#endif



#ifdef __cplusplus

}

#endif


#ifndef PM_INACK 

   #define PM_CLEAR  'X'

   #define PM_INACK  '0'  /* 非可靠推送 */

   #define PM_ACK    '1'  /* 可靠推送,需要对方接收后必须有接收确认回单 */

   #define PM_ANSWER '2'  /* 在特殊情况下使用，作为请求发送给目标点，外部处理结束后应答返回的时候，需要再次提交给原始的BUNIT */

#endif


#ifndef AM_AUTO

   #define AM_AUTO     'A'   // 一旦有数据就打包自动发送给请求客户端

   #define AM_CONFIRM  'C'   // 有数据只有在业务处理要求才发送给请求客户端,需要用AnswerData或AnswerDataPart才发送

#endif



#undef RAUPUB_API



#endif



