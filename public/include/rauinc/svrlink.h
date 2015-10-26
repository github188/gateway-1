// SvrLink.h: interface for the CSvrLink class.

//

//////////////////////////////////////////////////////////////////////



#if !defined(AFX_SVRLINK_H__6DD59251_58E5_47D1_B01B_F0DC850FC95C__INCLUDED_)

#define AFX_SVRLINK_H__6DD59251_58E5_47D1_B01B_F0DC850FC95C__INCLUDED_



#include "tcp.h"

#include "cemsg.h"

#include "raupub.h"

#include "comstru.h"



#ifdef RAUPUB_EXPORTS

#define RAUPUB_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define RAUPUB_API __declspec(dllimport)

#else

#define RAUPUB_API 

#endif

#endif





#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000



#define END_FIELD -1



typedef struct

{

   int sno;  // 首次调用ExtCall保留的souceno

   int dno;  // 首次调用ExtCall保留的destno

   int fno;  // 首次调用ExtCall保留的funcno

   int bno;  // 首次调用ExtCall保留的batchno

   ST_PACKHEAD ahead;  // 应答返回的头数据保留用

} ST_PDKEEP;


class RAUPUB_API CSvrLink  
{

public:

	CSvrLink();
	virtual ~CSvrLink();

	int  racBFuncNo;       
	int  racID;            
	int iThisProcID;        
	
	bool bExit;             
	ST_PACKHEAD head;       
	ST_PDKEEP xcdata; 
	unsigned int m_lastrcvtick;   // 上次从RAC收到消息时间点，请用tc2_tc1()进行判断

private:

   unsigned char iGRFrame;     // 帧号

   unsigned int iLastReqType;  // 上次或本次调用的功能号

   int iLastRetCode;  // 上次处理结果的成功或失败，用于通知RAC

   ST_PACKHEAD reqhead;    // 请求包的头保留，以便在业务功能中检查字段等属性

   char eMsg[256];

   CTcpSocket linksock;    // 与调度中心的TCPIP连接

   short svr_hs1;          // 调度中心（short）1，用于判断是否与本业务单元主机的字序相同

   char buf[MAXMSG+sizeof(MSGHEAD)+50];     // 包括起始的消息头

   char iAnswerMode;       // 当前有效的应答模式，见AM_AUTO / AM_CONFIRM

   TRUSERID ruserid;       // 本请求提交的客户端ID

   

public:

	int SetCol(TRUSERID *handle,...);

	int RACMsgLogOut(int logid, char *pFormat, ...);

	int Error_Deal(int retcode, char *omess, char *pFormat,...);

	void GetRequestHead(ST_PACKHEAD *phead);

	void GetAnswerHead(ST_PACKHEAD *phead);

	int RATransfer(ST_CPACK *rpack=NULL, ST_PACK *pArrays=NULL);

	int ExtTransfer(unsigned short destno, unsigned short funcno, ST_CPACK *rpack=NULL, ST_PACK *pArrays=NULL);

	int IntTransfer(char *szRAUGroupID, ST_CPACK *rpack=NULL,ST_PACK *pArrays=NULL);

	int RACallNext(int acktime, ST_CPACK *apack, ST_PACK *pArrays);


	int RACall(int acktime, ST_CPACK *rpack, ST_CPACK *apack, ST_PACK *pArrays);


	int IntCallNext(int waittime, ST_CPACK *apack, ST_PACK *pArrays);


	int IntCall(char *szRAUGroupID, int waittime, ST_CPACK *rpack, ST_CPACK *apack, ST_PACK *pArrays);


	int UpdateRegFunction(unsigned int reqtype,char *rtname,char *programmer,int priority=1,bool bstop=false);


	int SMInterlockedAdd(int smh,unsigned int offset,int increment=1);
   
	int SMRead(int smh, void *rbuf, unsigned int offset,unsigned int rblen,bool bRLock=false);
  
	int SMWrite(int smh,void *wdata,unsigned int offset,unsigned int length,bool bWLock=true);

	int SMOpenBlock(char *name,unsigned int length);

	int SMUnlock(int smh);


	int SMTryLock(int smh);
   

	int SMLock(int smh);

	int SetCol(TRUSERID * puserid, unsigned char *parmbits);

  

	int PutRow(TRUSERID * handle,ST_PACK *pdata,int retcode,char *rtnMsg);

	int Err_Deal(char *emsg, int retcode);



	int AnswerDataEx(TRUSERID *ruserid,ST_CPACK *apack,ST_PACK *pArrays,int retcode,char *szmsg);

	int AnswerData(TRUSERID *ruserid,int retcode,char *szmsg,unsigned char nextflag);


	int SetMaxRow(int maxrows);

	int SetAnswerMode(char iAnswerMode);

	bool CheckProcCancel();


	int ExtCallNext(int acktime, char *abuf, int &alen);

	int ExtCall(int sourceno, int destno, int funcno, int batchno, int acktime, void *rdata, int rdlen, char *abuf, int &alen);

	int PushData(int sourceno, int destno, int funcno, int batchno, char pushmode, int acktime, void  *pdata, int pdlen);

	int PushDataEx(TPUSHDEST *pushdests, int pdcount, void *pdata, int pdlen, char pushmode, int acktime);

 
	int ExtCallNext(int acktime,ST_CPACK *apack,ST_PACK *pArrays);

	int ExtCall(int sourceno,int destno,int funcno,int batchno,int acktime,ST_CPACK *rpack,ST_CPACK *apack,ST_PACK *pArrays);
  
	int PushDataEx(TPUSHDEST *pushdests,int pdcount,ST_CPACK *ppack,char pushmode,int acktime,ST_PACK *pArray=NULL);

	int PushData(int sourceno,int destno,int funcno,int batchno,ST_CPACK *ppack,char pushmode,int acktime,ST_PACK *pArray=NULL);

	int ResetAnswerData();

	void ResetNormalCPack(ST_CPACK *pack,unsigned char nextflag,int recCount);   

	int SetAnswerTimeout(unsigned int atimeout);

	int SetRAUError(int errcode,char *szErrMsg);
 
	int RACLogOut(char logtype,int logid,int textlen,char *text,int datalen=0,void *data=NULL);

	int PushRowEx(TPUSHDEST *pushdests,int pdcount,ST_PACK *pack,char pushmode,int batchno,int acktime);

	int PushRow(int sourceno,int destno,int funcno,ST_PACK *pack,char pushmode,int batchno,int acktime);

	int PutRowData(ST_PACK *pdata);

	bool CheckLink();

	bool LinkOK();

	int Processing(ST_RAUNIT *pBUnit);

	void Close();

	int ToLink(ST_RAUNIT *pBUnit);

private:

	int SetTRUserID(TRUSERID *ruserid);

	int SMLockCmd(int smh, short lockmode);

	int DataDone(int retcode,char *szMsg);

	int CallBPFunction(MSGBUF *pmb);

	int SendMsg(short msgtype,unsigned short msglen=0,void *msgbuf=NULL);

	int RecvMsg(int timeout=3000);

	MSGBUF * GetRecvMsg();

};



#undef RAUPUB_API



#endif // !defined(AFX_SVRLINK_H__6DD59251_58E5_47D1_B01B_F0DC850FC95C__INCLUDED_)

