#pragma once

#include "yt/util/tss.h"

/*
auto_ptr��[]�汾
*/

namespace yt
{
	class BufGuard
	{
		public:
			BufGuard(char *buf,bool flag);
			~BufGuard();
		private:
			char *m_buf;
			bool m_flag;
	};
}
