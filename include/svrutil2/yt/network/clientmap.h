#pragma once

#include "yt/util/seqmap.h"

namespace yt
{
	//�ͻ���id��Ӧ��map
	class ClientSocket;
	class ClientMap : public SeqMap<ClientSocket*>
	{
		public:
			void Close();
			ClientSocket* Get(int seq);
			void SendAll(const char *buf,size_t buflen);
			void SendSomeAll(int id,const char *buf,size_t buflen);
	};
}
