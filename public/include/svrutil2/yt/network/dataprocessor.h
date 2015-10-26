#pragma once

#include "yt/util/non_copyable.h"
#include "yt/log/log.h"
#include<string>
using namespace std;
namespace yt
{
	//NetProxyQueueThreadsProcess��ҵ���߼�������
	class ClientSocketBase;
	typedef void (*ParseCallBack)(void* client);
	class DataProcessor : public NonCopyable
	{
		public:
			virtual ~DataProcessor(){}
			virtual int Process(const char * /*inbuf*/,size_t /*inbuflen*/,string & /*outbuf*/){return 0;};
			virtual int Process(const char * /*inbuf*/,size_t /*inbuflen*/,string & /*outbuf*/,int /*clientid*/){return 0;};
			virtual int Process(const char * /*inbuf*/,size_t /*inbuflen*/,string & /*outbuf*/,int /*NetworkId*/,int /*clientid*/){return 0;};			
			ParseCallBack m_fun;
	};
	/*class DataProcessor_Echo : public DataProcessor
	{
		public:
			DataProcessor_Echo(int second = 0) : m_second(second){}
			virtual ~DataProcessor_Echo(){}
			virtual int Process(const char *inbuf,size_t inbuflen,string &outbuf){
				if(m_second > 0)
					sleep(m_second);
				AC_DEBUG("data arrived");
				size_t outlen = outbuflen;
				size_t len = 0;
				len += inbuflen;
				if(len < outlen)
				{
					memcpy(outbuf,inbuf,inbuflen);
					outbuflen = inbuflen;
					return 0;
				}
				return -1;
			}
		private:
			int m_second;
	};
	*/
}

