#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <memory>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <map>
#include <queue>
#include "yt/log/ostreamlogger.h"
#include "yt/log/log.h"
#include "yt/log/datefilelogger.h"
#include <yt/util/ptrsguard.h>
#include <yt/util/queue.h>
#include <yt/util/mutex.h>
#include "serverconf.h"
#include "ClientDataDispatch.h"
#include "ClientThread.h"
#include "TradeClient.h"
#include "protocol.h"
#include <unistd.h>

using namespace yt;
using namespace std;

//队列对象
yt::SyncQueue<std::string>   req_q;
yt::SyncQueue<std::string>   res_q;
//add by liuchengzhu 20150228
yt::SyncQueue<std::string>   exc_q;
//end add by liuchengzhu 20150228 

//add by wangzhen 
std::queue <CClientThread*>   ClientThread_q;   //存放登出线程，再利用

map<int,CClientThread *>  map_cs;
yt::ThreadMutex			  map_mutex;

MonitorInfo		monitorinfo;
void init_monitorinfo();

void prog_exit(int signo);
void InitMain(int argc,char **argv,bool& isdaemon);

char g_configfilename[128];

int main(int argc,char **argv)
{
	bool isdaemon = false;
	InitMain(argc,argv,isdaemon);

	AppLog gAppLog;
	gAppLog.InitLog("./",1,0);


	auto_ptr<OStreamLogger> stdoutlog(new(nothrow) OStreamLogger(std::cout));	
	AC_SET_DEFAULT_LOGGER(stdoutlog.get());

	//读取配置信息
	ServerConf* sconf = ServerConf::Instance();	
	if(sconf->Read(g_configfilename) != 0)
	{
		AC_ERROR("read %s failed!",g_configfilename);
		return 0;	
	}
	//设置log信息
	DateFileLogger *runlog = NULL;    
	DateFileLogger *errlog = NULL;    
	DateFileLogger *infolog = NULL;    
	if(isdaemon)    {            
		runlog = new(nothrow) DateFileLogger(string(sconf->m_logdir) + sconf->m_logprefix + ".run");            
		errlog = new(nothrow) DateFileLogger(string(sconf->m_logdir) + sconf->m_logprefix + ".error");            
		infolog = new(nothrow) DateFileLogger(string(sconf->m_logdir) + sconf->m_logprefix + ".sys");            
		AC_SET_DEFAULT_LOGGER(infolog);           
		AC_SET_LOGGER(yt::LP_USER1,runlog);            
		AC_SET_LOGGER(yt::LP_ERROR,errlog);   
	}   
	auto_ptr<DateFileLogger> g1(runlog);    
	auto_ptr<DateFileLogger> g2(errlog);    
	auto_ptr<DateFileLogger> g3(infolog);

	init_monitorinfo();

	bool b = ClientDataDispatch::Instance()->Init();
	if (!b)
	{
		AC_ERROR("ClientDataDispatch Init Failed!");
		return 0;
	}


	AC_ERROR("Program start!");

	ClientDataDispatch::Instance()->Join();

	AC_ERROR("Program exit!");

	return 0;
}

void InitMain(int argc,char **argv,bool& isdaemon)
{
	memset(g_configfilename,0,sizeof(g_configfilename));	
	int ch;	
	while((ch = getopt(argc,argv,"f:d"))!= -1)	
	{		
		switch(ch)		
		{			
			case 'f':				
				strncpy(g_configfilename,optarg,sizeof(g_configfilename));				
			break;			
			case 'd':				
				isdaemon = true;				
			break;		
		}	
	}
	if(strlen(g_configfilename) == 0)		
		strcpy(g_configfilename,"../conf/cfg.xml");		
	AC_INFO("configfilename = %s\n",g_configfilename);	
	if(isdaemon)	
	{		
		int pid;		
		signal(SIGCHLD, SIG_IGN);		
		pid = fork();		
		if(pid < 0) 
		{			
			perror("fork");			
			exit(-1);		
		}		
		else if(pid > 0)			
			exit(0);		
		setsid();				
		int fd;		
		fd = open("/dev/null",O_RDWR,0);		
		if (fd != -1)		
		{			
			dup2(fd,STDIN_FILENO);			
			dup2(fd,STDOUT_FILENO);			
			dup2(fd,STDERR_FILENO);			
			if (fd > 2)			
				close(fd);			
		}	
	}        
	//设置信号处理
	signal(SIGCHLD, SIG_DFL);        
	signal(SIGPIPE, SIG_IGN);        
	signal(SIGINT, prog_exit);        
	signal(SIGKILL, prog_exit);        
	signal(SIGTERM, prog_exit);		
}
void prog_exit(int /*signo*/)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGKILL, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	ClientDataDispatch::Instance()->Stop();

	req_q.Push("exit");
	res_q.Push("exit");
	//add by liuchengzhu 20150304
	exc_q.Push("exit");
	//end add by liuchengzhu 20150304
}



void init_monitorinfo()
{
	char chostname[64]={0};
	int  ilen=64;
	gethostname(chostname,ilen);
	string sGateway="gateway";
	string spname="";
	spname.append(chostname,strlen(chostname));
	spname.append(sGateway.c_str(),sGateway.length());
	

	memset(&monitorinfo,0,sizeof(monitorinfo));
	monitorinfo.Groupno=ServerConf::Instance()->m_groupno;
	memcpy(monitorinfo.Ip,ServerConf::Instance()->m_queryhost,sizeof(monitorinfo.Ip));
	monitorinfo.Port=0;
	memcpy(monitorinfo.AppName,spname.c_str(),spname.length());
	//增加启动时间
	monitorinfo.StartTime=time(NULL);
	//add by liuchengzhu 20150322
	monitorinfo.Funcno=ServerConf::Instance()->m_ASyncConnInfo.m_ASynFuncNo;
	//end add by liuchengzhu 20150322
}