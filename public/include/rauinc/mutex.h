// mutex.h: interface for the CMUTEX class.

//

//////////////////////////////////////////////////////////////////////



#if !defined(AFX_MUTEX_H__41E159AF_3A8F_471C_A5CB_03544AD54994__INCLUDED_)

#define AFX_MUTEX_H__41E159AF_3A8F_471C_A5CB_03544AD54994__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000



#ifdef WIN32

#include <windows.h>

#else

#include <pthread.h>

#endif

#ifdef RAUPUB_EXPORTS

#define RAUPUB_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define RAUPUB_API __declspec(dllimport)

#else

#define RAUPUB_API 

#endif

#endif



class RAUPUB_API CMUTEX  

{

public:

	int TryLock();

	void Lock();

	void UnLock();

	CMUTEX();

	virtual ~CMUTEX();

protected:

//public:

#ifdef WIN32

   HANDLE m_lock;

#else

   pthread_mutex_t m_lock;

#endif

};





class RAUPUB_API CPMUTEX  

{

public:

	CPMUTEX(CMUTEX *pmutex);

	virtual ~CPMUTEX();

private:

   CMUTEX *p_mutex;

};







class RAUPUB_API CMutexEvent : public CMUTEX  

{

public:

	void WaitEventTime(unsigned int wait_ms);

	void SetEvent();

	void WaitEvent();

	CMutexEvent();

	virtual ~CMutexEvent();



private:

#ifdef WIN32

   HANDLE m_event;

#else

   pthread_cond_t m_event;

#endif

};



#endif // !defined(AFX_MUTEX_H__41E159AF_3A8F_471C_A5CB_03544AD54994__INCLUDED_)

