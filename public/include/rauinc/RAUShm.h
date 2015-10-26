#if !defined(AFX_RAUSHM_H__24B9323D_EFD2_40DE_AC7B_C68A7B7F7566__INCLUDED_)

#define AFX_RAUSHM_H__24B9323D_EFD2_40DE_AC7B_C68A7B7F7566__INCLUDED_



#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000



#ifdef WIN32

#include <windows.h>

typedef int key_t;

typedef HANDLE SHMID;

#define INVALID_ID NULL

#else

/* *********** 

各个系统平台会根据需要调用这些头文件的

#include <sys/types.h>

#include <sys/ipc.h>

*******************************************************/

#include <sys/shm.h>

typedef int SHMID;

#define INVALID_ID -1

#endif



#ifdef KSSHMT_EXPORTS

#define KSSHMT_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define KSSHMT_API __declspec(dllimport)

#else

#define KSSHMT_API 

#endif

#endif



class KSSHMT_API CRAUShm  
{

private:

   int m_fileno;        // 用于加锁的文件, 也就是基本文件

   SHMID m_shmid;       // 共享内存ID

   int m_size;          // 本共享内存长度

   void *m_address;     // 共享内存空间在本进程中的地址

   char m_szmsg[256];   // 在错误的时候，用于存放错误信息



public:

	CRAUShm();
	virtual ~CRAUShm();

	bool Unlock(int offset=0, int len = -1);

	bool Lock(int offset=0,int len=-1);

	bool WriteLock(int offset=0,int len=-1);

	bool ReadLock(int offset=0,int len=-1);

	static bool Remove(const char *name);

	void * Open(const char *name, int shmsize);

	static key_t myftok(const char *path, int ID);

	void * GetSHMPtr();

	char * GetMessage();

	void Close();

private:

	bool locktype(int ltype, unsigned int offset, unsigned int  len);
	bool lockwitype(int ltype, unsigned int offset, unsigned int len);

};



#undef KSSHMT_API



#endif // !defined(AFX_RAUSHM_H__24B9323D_EFD2_40DE_AC7B_C68A7B7F7566__INCLUDED_)

