#ifndef __RMQLIB_H__
#define __RMQLIB_H__

#include "mytypes.h"

#define PROXY_SOCK5        3

typedef struct tag_rmqhandle
{
	char buf[28];
}TAG_RMQHANDLE;

#ifdef __cplusplus
extern "C" {
#endif


BOOL RmqInit(int *errcode,char *errormessage);

BOOL RmqSetProxy(int proxytype,char *ip,int port,char *user,char *pwd,int *errcode,char *errormessage);

int RmqConnect(char *ip,int port,int *errcode,char *errormessage,int handle=-1);

BOOL RmqReConnect(int handle,int *errcode,char *errormessage);

BOOL RmqAddFunction(int function,int *errcode,char *errormessage,int handle=0);

BOOL RmqDelFunction(int function,int *errcode,char *errormessage,int handle=0);

BOOL RmqBlind(int *errcode,char *errormessage,int handle=-1);

BOOL RmqLight(int *errcode,char *errormessage,int handle=-1);

int RmqCheckMessage(int *errcode,char *errormessage,int handle=-1,int timeout=-1);

int RmqCheckSlotMessage(int *errcode,char *errormessage,int handle=-1,int timeout=-1);

int RmqCheckAllMessage(int *type,int *errcode,char *errormessage,int handle=-1,int timeout=-1);

BOOL RmqPostMessage(int primary,int branchno,int function,char *message,int messagelen,
					 int *errcode,char *errormessage,int handle,int hook=0,
					 int ackmode=0,int zip=0);

BOOL RmqSendMessage(char *message,int messagelen,int *errcode,char *errormessage,
					 TAG_RMQHANDLE *rmqhandle,int handle=-1,int hook=0,int ackmode=0,
					 int primary=-1,int zip=0);

BOOL RmqTransMessage(int branchno,int function,char *message,int messagelen,
					  int *errcode,char *errormessage,TAG_RMQHANDLE *rmqhandle,
					  int handle=-1,int hook=0,int ackmode=0,int primary=-1,int zip=0);

BOOL RmqReadMessage(char *message,int messagebufsize,int* messagelen,int handle,
					 TAG_RMQHANDLE *rmqhandle,int *errcode,char *errormessage,
					 int timeout=-1);

BOOL RmqAttachServer(int branchno,int function,int handle,TAG_RMQHANDLE *rmqhandle,
					  int *errcode,char *errormessage);

BOOL RmqPing(int handle,int *errcode,char *errormessage);

BOOL RmqQueryRequest(int hi,int low,int cmd,int *errcode,char *errormessage,int handle);

int RmqQueryResult(char* message,int messagesize,int *messagelen,int *errcode,char *errormessage,int handle,int timeout=-1);

BOOL RmqGetHook(int handle,int *hook,int *errcode,char *errormessage);

BOOL RmqGetHandle(int handle,TAG_RMQHANDLE *rmqhandle,int *errcode,char *errormessage);

void RmqClose(int handle=-1,bool flag=false);

void RmqExit();

/*内部使用的涵数*/
int RmqListen(int port,int *errcode,char* errormessage);
int RmqAccept(int handle,int *errcode,char *errormessage);
BOOL RmqCheckSSL(int handle,int crypto,int *errcode,char *errormessage);
BOOL RmqIsLogined(int handle);
void RmqSetLogined(int handle,char *user,char *pwd);
BOOL RmqGetUserPwd(int handle,char *user,char *pwd);
BOOL RmqSetLocation(int handle,int location,int windowsize);
BOOL RmqGetLocation(int handle,int *location,int *windowsize);
int RmqNewHandle();
void RmqDelHandle(int handle);
int RmqPeerHandle(TAG_RMQHANDLE *rmqhandle);


BOOL RmqAddSlot(int slot,int *errcode,char *errormessage,int handle=0);

int RmqGetSlotHandle(int handle,int *errcode,char *errormessage,int timeout=-1);

BOOL RmqSlotSendMessage(char *message,int messagelen,int *errcode,char *errormessage,
						 int slothandle,int zip=0);

BOOL RmqSlotReadMessage(char *message,int messagebufsize,int* messagelen,int handle,
						 int *errcode,char *errormessage,int timeout=-1);

int RmqLinkSlot(char *ip,int port,int branchno,int slot,int *errcode,
				 char *errormessage,int timeout=-1);

BOOL RmqCloseConnect(int handle,int x,int y,int connectno,int timestamp,int *errcode,char *errormessage);

BOOL RmqCheckConnect(int handle,int x,int y,int connectno,int timestamp,int *errcode,char *errormessage);

void RmqGetConnectInfo(TAG_RMQHANDLE *rmqhandle,int *x,int *y,int *connectno,int *timestamp);

#ifdef __cplusplus
}
#endif

#endif
