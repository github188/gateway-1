
#ifndef _OPTIONAL_EXCEPTIONS_
#define _OPTIONAL_EXCEPTIONS_
//#include "mysql.h"
#include <string>
#include <iostream>
using namespace std;

class  OptionalExceptions
{
public:
	
	/// \param e if true, exceptions are enabled (this is the default)
	OptionalExceptions(bool e = true) :exceptions_(e)
	{

	}
	 virtual ~OptionalExceptions() { }

	void enable_exceptions() const { exceptions_ = true; }

	void disable_exceptions() const { exceptions_ = false; }

	bool throw_exceptions() const { return exceptions_; }

protected:
	
	void set_exceptions(bool e) const { exceptions_ = e; }

	void set_error_num(int error_no);
	void set_error_info(const char *errorInfo);

	int get_error_no();
	string get_error_info();

private:
	mutable bool exceptions_;
	int m_error_no;
	string m_error_info;
};

#endif
