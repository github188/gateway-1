#pragma once


//RMQ������ʼ����
class CRMQInit
{
public:
	CRMQInit();
	~CRMQInit();
};

//�ͻ��˷�ʽ

class CRmqCli
{
public:
	
	CRmqCli(const char *ip, int port);
	
	~CRmqCli();
	
	inline bool IsConnected() const { return m_Connected; }
	
	bool Connect();
	
	void Disconnect();
	
	int ReadMessage(char *buf, int size, int timeout=-1);
	
	int SendMessage(char *buf, int size, int iBranchNo, int iFuncNo);
	
private:
	char m_RmqIP[64];			//RMQ������ַ
	int  m_RmqPort;			    //RMQ����˿�
	int  m_Handle;				//RMQ���Ӿ��
	bool m_Connected;			//�Ƿ�������
};