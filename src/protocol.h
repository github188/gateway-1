#pragma once

typedef struct {
	unsigned char  bStartFlag; //Э�����ʼ��־
	unsigned char  bVer; //�汾��
	unsigned char  bEncryptFlag; //���ܱ�־(���������,��Ϊ0,��ֵ�����nt_crypt.h�е�ö��ֵ)
	unsigned char  bFrag; //�Ƿ��а���Ƭ
	unsigned short  wLen; //�ܰ���
	unsigned short  wCmd; //�����
	unsigned short  wSeq; //�������к�,ҵ��ʹ��,ת�����ٹ���
	unsigned short  wCrc; //Crc16У����
	unsigned int dwSID; //�ỰID,ǰ�˺ͺ�˷ֱ�ʹ�ò�ͬ��SID
	//unsigned char  sReserved[4]; //�����ֶ�
	unsigned short  wTotal;//�а���Ƭʱ����Ƭ����
	unsigned short  wCurSeq;//�а���Ƭʱ����Ƭ���
} NtPkgHead; //Э���ͷ
typedef struct {
	NtPkgHead stNtPkgHead;
	unsigned char   sBody[0];
} NtPkg; //Э���

typedef struct {
	int Groupno;		//���
	char Ip[32];		//IP��ַ
	int Port;			//Port
	char AppName[32];	//Ӧ�ó�������
	int RequestOrder;	//�µ���
	int  RequestQuery;	//��ѯ��
	int  RespDelegExcepCount;	//ί���쳣����
	int  RespQueryExcepCount;	//��ѯ�쳣����
	int ThreadCount;			//�����߳���
	int HeartKeepCount;			//����ά�ֹ����߳���
	long StartTime;				//����ʱ��
	//add by liuchengzhu 20150322
	int Funcno;					//�����ܺ�
	//end add by liuchengzhu 20150322

} MonitorInfo;  //ҵ������Ϣ