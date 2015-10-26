


#if !defined(AFX_BDEFINE_H__DC4F6CAA_3CE4_438A_8DCF_D92AF3610688__INCLUDED_)

#define AFX_BDEFINE_H__DC4F6CAA_3CE4_438A_8DCF_D92AF3610688__INCLUDED_





#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#include "cemsg.h"

#include "raupub.h"

#include "comstru.h"

#include "svrlink.h"

#include "logfile.h"



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



// 所有业务处理过程：

// 返回  : int 0 表示成功处理，其他作为处理失败

// 参数  : TRUSERID *handle 本请求数据的通讯请求端标识

// 参数  : ST_PACK *rPack 业务处理请求数据

// 参数  : int *iRetCode 处理结果的返回代码，0表示成功，其他就是处理失败，在本业务处理函数调用之前，已经被初始化为0

// 参数  : char *szMsg 处理结果的返回信息，在本业务处理函数调用之前，已经被初始化为空""

typedef int (* APPFUNC)(TRUSERID *handle,int iRequest,ST_PACK *rPack,int *iRetCode,char *szMsg); 



#define INSERT_FUNCTION(funcno,func,comment,coder,pr) {funcno,func,comment,coder,pr,false,0,0,0,0,0},



typedef struct 

{

   unsigned int RequestType;  // 请求功能号

   APPFUNC  pFunc;            // 本功能函数指针，即调用入口地址

   char szRTName[80];         // 本功能业务名称

   char szProgrammer[20];     // 编程人员名

   int  iPriority;            // 优先级 目前分为1,2,3 自高到低

                              /* 同时也决定其处理的优先级

                                           1 - 交易类（会修改数据的请求）或 关键查询业务 ...

                                           2 - 单笔返回数据查询

                                           3 - 多笔返回数据查询

                              */

   bool bStoped;              // 是否暂时停止本业务

   unsigned int nSuccess;     // 处理成功次数

   unsigned int nFail;        // 处理失败次数

   double       dTakentime;   // 处理成功累计时间 以毫秒计，用于测试在本RAC下该业务的处理速度

   unsigned int nTt_max;      // 最大处理时间

   unsigned int nTt_min;      // 最小处理时间

} TBDefine;



extern TBDefine *g_BDefines;



typedef int (*XPROCESS_M)(TRUSERID *handle,char *rdata,int rdlen,int *iRetCode, char *szMsg);

extern XPROCESS_M pProcess;



typedef int (*XCALLBDFUNC_M)(int fno,TRUSERID *handle,ST_CPACK *rpack,ST_PACK *pArrays,int *iRetCode, char *szMsg);

extern XCALLBDFUNC_M pCallBDFunc;

typedef int (*XWRITEAPPFUNC)(int racBaseFuncNo,int racID);

extern XWRITEAPPFUNC pWriteAppInfo;


int CallBDFunc(int fno,TRUSERID *handle,ST_CPACK *rpack,ST_PACK *pArrays,int *iRetCode, char *szMsg);


int WriteAppInfo(int racBaseFuncNo,int racID);


RAUPUB_API int ResetBPFunctions();


RAUPUB_API int ListBPFunctions(char *bpfile);

RAUPUB_API int FindBPFunction(unsigned int reqtype);





#ifdef __cplusplus

}

#endif


RAUPUB_API CSvrLink *RAUPubInitialize(TBDefine *pBDefine,XCALLBDFUNC_M pCallFunc,XWRITEAPPFUNC pWriteApp,CLogFile *pLogFile,XPROCESS_M process=NULL);



#undef RAUPUB_API



#endif // !defined(AFX_BDEFINE_H__DC4F6CAA_3CE4_438A_8DCF_D92AF3610688__INCLUDED_)

