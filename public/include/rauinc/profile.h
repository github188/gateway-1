/********************************************************

	读取配置文件类

	文件名: profile.h

	说明: 

	创建: 

	修改: 吴舸 2001-04-20

	修改原因: 规范编码

**********************************************************/

#ifndef __PROFILE_H

#define __PROFILE_H



typedef void (*FUNC_INI2LOG)(const char *fmt,...);



#ifdef RAUPUB_EXPORTS

#define RAUPUB_API __declspec(dllexport)

#else

#ifdef _WINDOWS

#define RAUPUB_API __declspec(dllimport)

#else

#define RAUPUB_API 

#endif

#endif



RAUPUB_API void SetINI2LOGFunc(FUNC_INI2LOG func);



class RAUPUB_API TIniFile

{

	//输入输出流

   FILE *fin,*fout;

	//返回指向section名称的字符串指针

	char *titlePos( char *buf, int *len );

	//是否是一个section字符串

	bool isTitleLine( char *bufPtr );

	//是否是指定的section

	bool containTitle( char *buf, const char *section );

	//移动文件指针到相应的section位置

	bool gotoSection(const char *section );

	//返回指向entry名称的字符串指针

	char *textPos( char *buf, const char *entry );

	//去除字符串前面的空格

	void stripQuotationChar( char *buf );

	//读取一个entry的值

	int readEntry( const char *entry, char *buf, int bufSize, bool strip );

	//写一个entry





public:

   char * ReadTString(const char *section,const char *entry,const char *defaultString,char *buffer,int   bufLen);


   static char * LRTrim(char *str);

	//配置文件名称

	char FileName[128];

	//打开配置文件

	bool Open(const char *filename);

	//读取一个int型entry的值

	int ReadInt( const char *section, const char *entry, int defaultInt);

	//读取一个string型的entry的值

	int ReadString( const char *section, const char *entry,const char *defaultString, char *buffer,int bufLen);

	//写一个string型的entry到指定的section中

	//写一个int型的entry到指定的section中

	//关闭配置文件

	void Close();



   TIniFile();

	~TIniFile();

private:

	char * FGetS(char *pbuf,int size,FILE *fp);

	void out_close();

	void in_close();

	char * percolate(char *str);

};



#undef RAUPUB_API



#endif

