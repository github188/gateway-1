// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "Thread.h"
#include "yt/log/ostreamlogger.h"
#include "yt/log/log.h"
#include "yt/log/datefilelogger.h"

unsigned int  g_nThreadStackSize = 10240000;

#ifdef WIN32
DWORD WINAPI _ThreadEntry(LPVOID pParam)
#else
void * _ThreadEntry(void *pParam)
#endif
{
	CThread *pThread = (CThread *)pParam;
	try 
  	{
#ifdef WIN32
		pThread->m_dwThreadId = ::GetCurrentThreadId();
	
#endif
		if(pThread->InitInstance()){
			pThread->Run();
		}
	}
	
	catch (...)
	{
		AC_ERROR("异常运行线程结束\n");
	}

	pThread->ExitInstance();
	return 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CThread::CThread()
{
	m_hThread = 0;
}


CThread::~CThread()
{
#ifdef WIN32
	if (m_hThread) ::CloseHandle(m_hThread);
#endif
}


bool  CThread::Create()
{
	bool ret = true;
#ifdef WIN32
	DWORD IDThread;
	m_hThread = ::CreateThread(NULL,0,_ThreadEntry,this,0,&IDThread);
	if(m_hThread==NULL){
		ret = false;
	}
#else
	pthread_attr_t  attr;
	pthread_attr_init(&attr);
#ifdef HP_UX
	//pthread_attr_setstacksize(&attr,102400000);
	size_t size;
	size=ks_GetStackSize();
	pthread_attr_setstacksize(&attr,size);
#endif
	ret = (::pthread_create(&m_hThread,&attr,&_ThreadEntry , this)==0);
#endif
	return ret;
}


bool CThread::InitInstance()
{
	return true;
}


void CThread::ExitInstance()
{
//	pthread_exit(NULL);
}


THREAD_HANDLE CThread::GetCurrentHandle()
{
#ifdef WIN32
	return ::GetCurrentThread();
#else
	return ::pthread_self();
#endif
}

void CThread::Join()
{
#ifdef WIN32
	WaitForSingleObject(m_hThread, INFINITE);
#else
	pthread_join(m_hThread, NULL);
#endif
}