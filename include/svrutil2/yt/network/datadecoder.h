#pragma once

#include <sys/types.h>
#include <yt/util/queue.h>
#include <yt/util/tss.h>
#include <map>


using namespace std;



namespace yt
{

	void Clear_Flag(void *);

	//���ֽ�ճ������
	enum
	{
		PROTOCOLTYPE_TEXT = 1,
		PROTOCOLTYPE_BINARY,
		PROTOCOLTYPE_MSG,
		PROTOCOLTYPE_YHT,
		PROTOCOLTYPE_TJ,
		PROTOCOLTYPE_HJ,
		PROTOCOLTYPE_HTTP,		
	};
	enum
	{
		HEADER_LEN_2 = 2,
		HEADER_LEN_4 = 4,
		HEADER_LEN_6 = 6,
		HEADER_LEN_8 = 8,
		HEADER_LEN_20 = 20,
	};
	class ClientSocketBase;
	class DataDecoderBase
	{
		public:
			DataDecoderBase(){}
			virtual ~DataDecoderBase(){}
			virtual void Process(ClientSocketBase *pClient,const char *buf,size_t buflen) = 0;
			virtual int OnPackage(ClientSocketBase *pClient,const char *buf,size_t buflen) = 0;
			virtual void OnClientClose(ClientSocketBase * /*pClient*/){};
			virtual void OnAcceptClient(ClientSocketBase * /*pClient*/){};
	};
	class DataDecoder_helloworld : public DataDecoderBase
	{
		public:
			virtual ~DataDecoder_helloworld(){}
			virtual void Process(ClientSocketBase *pClient,const char *buf,size_t buflen);
			virtual int OnPackage(ClientSocketBase *pClient,const char *buf,size_t buflen);
	};
	class DataDecoder : public DataDecoderBase
	{
		public:
			DataDecoder(int pttype,size_t hdlen,size_t maxonepacklen/*һ�������Ĵ�С,����ͷ�Ĵ�С*/) : m_pttype(pttype),m_hdlen(hdlen),m_maxonepacklen(maxonepacklen) {}
			virtual ~DataDecoder(){}
			virtual void Process(ClientSocketBase *pClient,const char *buf,size_t buflen);
			virtual size_t GetBuflen(const char *buf,size_t leftbuflen);
		protected:
			const int m_pttype;
			const size_t m_hdlen;
			const size_t m_maxonepacklen;
	};
	class DataDecoder_DFix : public DataDecoderBase	//DFIXЭ���ͷ��L=�Ľ����
	{
		static const char SOH = 0x1;
		public:
			DataDecoder_DFix(size_t maxonepacklen) : m_maxonepacklen(maxonepacklen){}
			virtual ~DataDecoder_DFix(){}
			virtual void Process(ClientSocketBase *pClient,const char *buf,size_t buflen);
		protected:
			const size_t m_maxonepacklen;
	};
	class DataDecoder_Q : public DataDecoder
	{
		public:
			DataDecoder_Q(size_t maxonepacklen) : DataDecoder(PROTOCOLTYPE_MSG,HEADER_LEN_4,maxonepacklen){}
			DataDecoder_Q(size_t maxonepacklen,int pttype,size_t hdlen) : DataDecoder(pttype,hdlen,maxonepacklen){}
			inline void SetQueue(SyncQueue<pair<int,string>* > *processqueue){m_processqueue = processqueue;}
			virtual ~DataDecoder_Q(){}
			virtual int OnPackage(ClientSocketBase *pClient,const char *buf,size_t buflen);
//		private:
			SyncQueue<pair<int,string>* > *m_processqueue;
	};
	class DataDecoder_Q2 : public DataDecoder_Q
	{
		public:
			DataDecoder_Q2(size_t maxonepacklen) : DataDecoder_Q(maxonepacklen){}
			DataDecoder_Q2(size_t maxonepacklen,int pttype,size_t hdlen) : DataDecoder_Q(maxonepacklen,pttype,hdlen){}		
			void SetQueue(SyncQueue<pair<int, pair<int,string>*> > *processqueue){ m_processqueue = processqueue; }
			void SetId(size_t threadid) { m_threadid = threadid; }
			
			virtual int OnPackage(ClientSocketBase *pClient,const char *buf,size_t buflen);
			
		private:
			size_t m_threadid;
			SyncQueue<pair<int, pair<int,string>*> > *m_processqueue;
	};
	class ThreadCluster;
	class DataDecoder_Cluster: public DataDecoder
	{
		public:
			DataDecoder_Cluster(size_t maxonepacklen) : DataDecoder(PROTOCOLTYPE_BINARY,HEADER_LEN_4,maxonepacklen),m_threadcluster(NULL){}
			
			void SetThreadCluster(ThreadCluster *threadcluster){ m_threadcluster = threadcluster; }
			virtual int OnPackage(ClientSocketBase *pClient,const char *buf,size_t buflen);
			
		private:
			ThreadCluster *m_threadcluster;
	};
}

