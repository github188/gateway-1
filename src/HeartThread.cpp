#include <yt/log/log.h>
#include <yt/util/queue.h>
#include <yt/util/mutex.h>
#include <yt/util/lockguard.h>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
//add by liuchengzhu 20150213
#include <string.h>
//end add by liuchengzhu 20150213
//add by liuchengzhu 20150413
#include "constant.h"
//end add by liuchengzhu 20150413
#include "protocol.h"
#include "ClientThread.h"
#include "HeartThread.h"
#include "serverconf.h"
#include "bits.h"

using namespace yt;
using namespace std;

extern map<int,CClientThread *>  map_cs;
extern yt::ThreadMutex			 map_mutex;
extern yt::SyncQueue<std::string>   exc_q;
extern MonitorInfo		monitorinfo;

bool CHeartThread::Init()
{
	m_sleeptime=ServerConf::Instance()->m_TimePara.sleeptime;
	m_stoptime=ServerConf::Instance()->m_TimePara.stoptime;
	m_outtime=ServerConf::Instance()->m_TimePara.outtime;

	return true;
}

void CHeartThread::SendStartUp()
{
	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return ;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"groupno",json_object_new_int(monitorinfo.Groupno));

	json_object_object_add(RspObj,"ip",json_object_new_string(monitorinfo.Ip));

	json_object_object_add(RspObj, "pn", json_object_new_string(monitorinfo.AppName));

	json_object_object_add(RspObj,"Funcno",json_object_new_int(monitorinfo.Funcno));

	strData=json_object_to_json_string(RspObj);

	string sRes;
	NtPkgHead mHead;
	memset((void *)&mHead,0,sizeof(NtPkgHead));
	mHead.bStartFlag=0xff;
	mHead.dwSID=htonl(0);
	mHead.wCmd=htons(6009);
	mHead.wLen=htons(sizeof(NtPkgHead)+strData.length());

	sRes.append((const char *)&mHead,sizeof(NtPkgHead));
	sRes.append(strData.c_str(),strData.length());

	exc_q.Push(sRes);
}


void CHeartThread::SendMonitor(long tcur)
{
	monitorinfo.ThreadCount=map_cs.size();

	int RunTime=tcur-monitorinfo.StartTime;

	string strData;
	json_object *RspObj = json_object_new_object();
	if(NULL == RspObj || is_error(RspObj))
	{
		AC_ERROR("%s", "json_object_new_object failed!");
		return ;
	}
	JsonObjGuard RspObjGuard(RspObj);

	json_object_object_add(RspObj,"Groupno",json_object_new_int(monitorinfo.Groupno));

	json_object_object_add(RspObj,"Ip",json_object_new_string(monitorinfo.Ip));

	json_object_object_add(RspObj,"Port",json_object_new_int(monitorinfo.Port));

	json_object_object_add(RspObj, "AppName", json_object_new_string(monitorinfo.AppName));

	json_object_object_add(RspObj,"RequestOrder",json_object_new_int(monitorinfo.RequestOrder));

	json_object_object_add(RspObj,"RequestQuery",json_object_new_int(monitorinfo.RequestQuery));

	json_object_object_add(RspObj,"RespDelegExcepCount",json_object_new_int(monitorinfo.RespDelegExcepCount));

	json_object_object_add(RspObj,"RespQueryExcepCount",json_object_new_int(monitorinfo.RespQueryExcepCount));

	json_object_object_add(RspObj,"ThreadCount",json_object_new_int(monitorinfo.ThreadCount));

	json_object_object_add(RspObj,"HeartKeepCount",json_object_new_int(monitorinfo.HeartKeepCount));

	json_object_object_add(RspObj,"RunTime",json_object_new_int(RunTime));

	strData=json_object_to_json_string(RspObj);

	string sRes;
	NtPkgHead mHead;
	memset((void *)&mHead,0,sizeof(NtPkgHead));
	mHead.bStartFlag=0xff;
	mHead.dwSID=htonl(0);
	mHead.wCmd=htons(6005);
	mHead.wLen=htons(sizeof(NtPkgHead)+strData.length());

	sRes.append((const char *)&mHead,sizeof(NtPkgHead));
	sRes.append(strData.c_str(),strData.length());

	exc_q.Push(sRes);
}

void CHeartThread::Run()
{
	//启动汇报服务号
	SendStartUp();

	string sreq;

	NtPkgHead mHead;
	memset((void *)&mHead,0,sizeof(NtPkgHead));
	mHead.dwSID=htonl(0);
	mHead.wCmd=htons(0);
	mHead.wLen=htons(sizeof(NtPkgHead));
	sreq.append((const char *)&mHead,sizeof(NtPkgHead));


	while(m_bRun)
	{

		vector<int> vUid;
		map<int,CClientThread *>::iterator it;

		time_t tcur=time(NULL);
		monitorinfo.HeartKeepCount=0;

		{
			map_mutex.Acquire();
			LockGuard lg(&map_mutex);

			for(it=map_cs.begin();it!=map_cs.end();it++)
			{
				int uid=it->first;
				CClientThread *pc=it->second;
				//退出线程清理
				if(pc->IsExit())
				{
					//add by liuchengzhu 20150408
					pc->Join();
					//end add by liuchengzhu 20150408
					delete pc;
					vUid.push_back(uid);
					continue;
				}
				//超过5min未有请求的线程终止
				if(((pc->m_tc.tRequest!=0)&&(tcur-pc->m_tc.tRequest>m_stoptime))||(pc->m_tc.bNeedStop==true))
				{
					if(pc->m_tc.bNeedStop==true)
					{
						//AC_ERROR("uid=%d,pc->m_tc.bNeedStop==true",uid);
					}
					else
					{
						//AC_ERROR("uid=%d,tcur-pc->m_tc.tRequest=%d,tcur=%ld,pc->m_tc.tRequest=%ld",uid,tcur-pc->m_tc.tRequest,tcur,pc->m_tc.tRequest);
					}

					//设置停止标志 以便心跳处理时终止进程
					string sreq2;
					NtPkgHead mHead2;
					memset((void *)&mHead2,0,sizeof(NtPkgHead));
					mHead2.dwSID=htonl(uid);
					mHead2.wCmd=htons(TID_DISCONNECT);
					mHead2.wLen=htons(sizeof(NtPkgHead));
					sreq2.append((const char *)&mHead2,sizeof(NtPkgHead));

					pc->Stop();
					pc->Push(sreq2);
				}
				//超过15min未有请求的线程统计个数
				if((pc->m_tc.tRequest!=0)&&(tcur-pc->m_tc.tRequest>m_outtime))
				{
					monitorinfo.HeartKeepCount++;
				}

				if(pc!=NULL)
				{
					pc->Push(sreq);
				}
				else
				{
					AC_ERROR("uid[%d] CClientThread is NULL\n",uid);
				}
			}
			for(unsigned int i=0;i<vUid.size();i++)
				map_cs.erase(vUid[i]);
		}
		//发送监控数据
		SendMonitor(tcur);

		sleep(m_sleeptime);
			
	}
}


