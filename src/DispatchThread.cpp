#include <yt/log/log.h>
#include <yt/util/queue.h>
#include <yt/util/mutex.h>
#include <map>
#include <string>
#include <arpa/inet.h>
#include "protocol.h"
#include "DispatchThread.h"
#include "ClientThread.h"
#include "constant.h"
// add by wangzhen
#include <yt/util/lockguard.h>
//end
using namespace yt;
using namespace std;

extern yt::SyncQueue<std::string>   req_q;
extern map<int,CClientThread *>  map_cs;
extern yt::ThreadMutex			  map_mutex;

bool CDispatchThread::Init()
{
	return true;
}

void CDispatchThread::Run()
{
	while(m_bRun)
	{
		string sreq;
		int ret=req_q.Pop(sreq,true);
		if(0==ret)
		{
			//add by liuchengzhu 20140108
			//退出等异常消息
			if(sreq.length()<sizeof(NtPkgHead))
				break;
			//end add by liuchengzhu 20140108

			NtPkgHead *phead=(NtPkgHead *)sreq.c_str();
			unsigned int uid=ntohl(phead->dwSID);
			unsigned short wCmd=ntohs(phead->wCmd);

			AC_INFO("uid=%d",uid);

			map<int,CClientThread *>::iterator it;
			CClientThread *pc=NULL;
			
			map_mutex.Acquire();
			LockGuard lg(&map_mutex);  //add by wangzhen 20150909

			it=map_cs.find(uid);

			switch(wCmd)
			{
			case TID_LOGIN:
				pc=new(nothrow) CClientThread;
				if(pc!=NULL)
				{
					map_cs[uid]=pc;
					pc->Init(uid);
					pc->Create();
					pc->Push(sreq);
				}
				else
				{
					AC_ERROR("new CClientThread uid[%d] fail\n",uid);
				}
				break;
			case TID_CLOSE:
				pc=it->second;
				if(pc!=NULL)
				{
					pc->Stop();
				}				
				break;
			default:
				if(it!=map_cs.end())
				{
					pc=it->second;
					if(pc!=NULL)
					{
						pc->Push(sreq);
					}
					else
					{
						AC_ERROR("uid[%d] CClientThread is NULL\n",uid);
					}
				}
				
			}
		  // map_mutex.Release();   //delete by wangzhen
		}
	}
}
