#pragma once
#include "RAUShm.h"
#include "json.h"

class JsonObjGuard
{
public:
	JsonObjGuard(json_object *obj) : m_obj(obj){}
	~JsonObjGuard(){
		if(NULL != m_obj)
			json_object_put(m_obj);
	}
private:
	json_object *m_obj;
};

class CShmWLockGuard
{
public:
	CShmWLockGuard(CRAUShm* p = NULL):m_phs(p)
	{
		if (NULL != m_phs)
			m_phs->WriteLock();
	}
	~CShmWLockGuard()
	{
		if (NULL != m_phs)
			m_phs->Unlock();
	}

private:
	CRAUShm* m_phs;
};

class CShmRLockGuard
{
public:
	CShmRLockGuard(CRAUShm* p = NULL):m_phs(p)
	{
		if (NULL != m_phs)
			m_phs->ReadLock();
	}
	~CShmRLockGuard()
	{
		if (NULL != m_phs)
			m_phs->Unlock();
	}

private:
	CRAUShm* m_phs;
};