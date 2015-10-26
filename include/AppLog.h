#ifndef APPLOG_H
#define APPLOG_H
#include <stdio.h>
#include <string>

#include "TradeCommon.h"


class SDKDLL_API AppLog {
public:
	AppLog();
	~AppLog();

public:
	// nFile :  0 -- off   1 -- on
	// nTerm :  0 -- off   1 -- on
	int InitLog(std::string strLogPath, int nFile, int nTerm);

	int ReleaseLog();

};

#endif //APPLOG_H
