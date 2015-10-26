#include <yt/log/log.h>
#include "stdio.h"
#include "string.h"
#include "BaseHandle.h"
#include "bits.h"

CBaseHandle::CBaseHandle(void)
{
}

CBaseHandle::~CBaseHandle(void)
{
}


void CBaseHandle::GetJsonField(json_object* obj, const char* filedname, int& f)
{
	json_object* tobj = json_object_object_get(obj, filedname);
	//add by liuchengzhu 20150518
	if((NULL == tobj) || (is_error(tobj)) || (json_type_int != json_object_get_type(tobj))){
		AC_ERROR("error json str %s:%s ",filedname,json_object_to_json_string(obj));
		return ;
	}
	//end add by liuchengzhu 20150518
	if (json_type_int == json_object_get_type(tobj))
	{
		f = json_object_get_int(tobj);
	}
	else
	{
		char acMsg[512];
		snprintf(acMsg, sizeof(acMsg), "%s:%d json_object_get_type error!", filedname, json_object_get_type(tobj));
		AC_ERROR("%s",acMsg);
	}
}

void CBaseHandle::GetJsonField(json_object* obj, const char* filedname, unsigned int& f)
{
	json_object* tobj = json_object_object_get(obj, filedname);
	//add by liuchengzhu 20150518
	if((NULL == tobj) || (is_error(tobj)) || (json_type_int != json_object_get_type(tobj))){
		AC_ERROR("error json str %s:%s ",filedname,json_object_to_json_string(obj));
		return ;
	}
	//end add by liuchengzhu 20150518
	if (json_type_int == json_object_get_type(tobj))
	{
		f = json_object_get_int(tobj);
	}
	else
	{
		char acMsg[512];
		snprintf(acMsg, sizeof(acMsg), "%s:%d json_object_get_type error!", filedname, json_object_get_type(tobj));
		AC_ERROR("%s",acMsg);
	}
}

void CBaseHandle::GetJsonField(json_object* obj, const char* filedname, double& f)
{
	json_object* tobj = json_object_object_get(obj, filedname);
	//add by liuchengzhu 20150518
	if((NULL == tobj) || (is_error(tobj)) || (json_type_double != json_object_get_type(tobj)&&json_type_int != json_object_get_type(tobj))){
		AC_ERROR("error json str %s:%s ",filedname,json_object_to_json_string(obj));
		return ;
	}
	//end add by liuchengzhu 20150518
	if (json_type_double == json_object_get_type(tobj))
	{
		f = json_object_get_double(tobj);
	}
	else if (json_type_int == json_object_get_type(tobj))
	{
		f = json_object_get_int(tobj);
	}
}

void CBaseHandle::GetJsonField(json_object* obj, const char* filedname, string& f)
{
	json_object* tobj = json_object_object_get(obj, filedname);
	//add by liuchengzhu 20150518
	if((NULL == tobj) || (is_error(tobj)) || (json_type_string != json_object_get_type(tobj))){
		AC_ERROR("error json str %s:%s ",filedname,json_object_to_json_string(obj));
		return ;
	}
	//end add by liuchengzhu 20150518
	if (json_type_string == json_object_get_type(tobj))
	{
		f = json_object_get_string(tobj);
	}
	else
	{
		char acMsg[512];
		snprintf(acMsg, sizeof(acMsg), "%s:%d json_object_get_type error!", filedname, json_object_get_type(tobj));
		AC_ERROR("%s",acMsg);
	}
}

void CBaseHandle::GetJsonField(json_object* obj, const char* filedname, unsigned char& f)
{
	json_object* tobj = json_object_object_get(obj, filedname);
	//add by liuchengzhu 20150518
	if((NULL == tobj) || (is_error(tobj)) || (json_type_string != json_object_get_type(tobj))){
		AC_ERROR("error json str %s:%s ",filedname,json_object_to_json_string(obj));
		return ;
	}
	//end add by liuchengzhu 20150518
	if (json_type_string == json_object_get_type(tobj))
	{
		memcpy((char*)&f, json_object_get_string(tobj), 1);
	}
	else
	{
		char acMsg[512];
		snprintf(acMsg, sizeof(acMsg), "%s:%d json_object_get_type error!", filedname, json_object_get_type(tobj));
		AC_ERROR("%s",acMsg);
	}
}
