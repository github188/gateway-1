#pragma once

#include "yt/util/non_copyable.h"
#include "yt/network/reactor.h"
#include <unistd.h>
#include <sys/time.h>

namespace yt
{
	//�¼�������Ļ���
	class EventHandler : public NonCopyable
	{
		public:
			EventHandler(){}
			inline void SetReactor(Reactor *pReactor){m_pReactor = pReactor;}
			virtual ~EventHandler(){}
			inline Reactor* GetReactor(){return m_pReactor;}
			virtual void Close() = 0;
		protected:
			Reactor *m_pReactor;
	};
	//����ʱ���¼�������
	class IdleEventHandler : virtual public EventHandler
	{
		public:
			IdleEventHandler(Reactor *pReactor){
				SetReactor(pReactor);
			}
			virtual ~IdleEventHandler(){}
			virtual void OnRun() = 0;
			inline void RegisterIdle()
			{
				GetReactor()->RegisterIdle(this);
			}
			inline void UnRegisterIdle()
			{
				GetReactor()->UnRegisterIdle(this);
			}
			virtual void Close()
			{
				GetReactor()->UnRegisterIdle(this);
			}
	};
	//ʱ�ӵ��¼�������
	class TMEventHandler : virtual public EventHandler
	{
		public:
			TMEventHandler(){};
			TMEventHandler(Reactor *pReactor){
				SetReactor(pReactor);
			}
			virtual ~TMEventHandler(){}
			virtual void OnTimeOut() = 0;
			inline void RegisterTimer(struct timeval *timeout)
			{
				UnRegisterTimer();
				to.tv_sec = timeout->tv_sec;
				to.tv_usec = timeout->tv_usec;
				gettimeofday(&regtime,NULL);
                                endtime.tv_sec=to.tv_sec + regtime.tv_sec;
				endtime.tv_usec=to.tv_usec + regtime.tv_usec;
				GetReactor()->RegisterTimer(this);
			}
			inline void RegisterTimer(int timeout)	//ע��ʱ��
			{
				UnRegisterTimer();
				to.tv_sec = timeout;
				to.tv_usec = 0;
				gettimeofday(&regtime,NULL);
                                endtime.tv_sec=to.tv_sec + regtime.tv_sec;
				endtime.tv_usec=regtime.tv_usec;
				GetReactor()->RegisterTimer(this);
			}
			inline void UnRegisterTimer()	//ע��ʱ��
			{
				GetReactor()->UnRegisterTimer(this);
			}
			virtual void Close()
			{
				GetReactor()->UnRegisterTimer(this);
			}
		public:
			struct timeval to;
                        struct timeval regtime;
			struct timeval endtime;
	};
	//�ļ����������¼�������
	class FDEventHandler : virtual public EventHandler
	{
		public:
			FDEventHandler(Reactor *pReactor){
				SetReactor(pReactor);
			}
			FDEventHandler(){}
			virtual ~FDEventHandler(){}
			inline void SetFD(int fd){m_fd = fd;}
			virtual void OnFDRead() = 0;
			virtual void OnFDWrite() = 0;
			virtual void OnFDReadTimeOut() = 0;
			virtual void OnFDWriteTimeOut() = 0;
			inline int RegisterRead(struct timeval *timeout)
			{
				Reactor *p = GetReactor();
				return p->RegisterReadEvent(this,timeout);
			}
			inline int RegisterRead(int timeout)
			{
				struct timeval timeout_;
				timeout_.tv_sec = timeout;
				timeout_.tv_usec = 0;

				return GetReactor()->RegisterReadEvent(this,&timeout_);
			}
			inline int RegisterWrite(struct timeval *timeout)
			{
				return GetReactor()->RegisterWriteEvent(this,timeout);
			}
			inline int RegisterWrite(int timeout)
			{
				struct timeval timeout_;
				timeout_.tv_sec = timeout;
				timeout_.tv_usec = 0;

				return GetReactor()->RegisterWriteEvent(this,&timeout_);
			}
			inline int UnRegisterRead()
			{
				return GetReactor()->UnRegisterReadEvent(this);
			}
			inline int UnRegisterWrite()
			{
				return GetReactor()->UnRegisterWriteEvent(this);
			}
			inline int GetFD() const {return m_fd;}
			int SetNonBlocking(bool blocking = true);
			static int SetNonBlocking(int fd,bool blocking = true);	
			virtual void Close()
			{
				GetReactor()->UnRegisterEvent(this);
				::close(m_fd);
				m_fd = -1;
			}
		protected:
			int m_fd;
	};
/*
	class TimerTest : public TMEventHandler
	{
		public:
			TimerTest(Reactor *pReactor) : TMEventHandler(pReactor){}
			virtual void OnTimeOut()
			{
				AC_INFO("time out");
				struct timeval timeout = {3,0};
				Register(&timeout);
			}
	};
*/
/*
	class SNEventHandler : public EventHandler
	{
		public:
			SNEventHandler()
			{
			}
			SNEventHandler(Reactor *pReactor,int signaltype) : EventHandler(pReactor),m_signaltype(signaltype)
			{
			}
			virtual ~SNEventHandler()
			{
			}
			int Register();
			virtual void OnSignal() = 0;
			virtual void Close();
		protected:
			int m_signaltype;
	};
*/
}

