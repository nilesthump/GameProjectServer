#include "Util.h"

namespace GameProjectServer
{

#ifndef __Linux__
	DWORD getThreadId()
	{
		return GetCurrentThreadId();
	}
#else
	pid_t getThreadId()
	{
		return syscall(SYS_gettid);
	}
#endif // !__Linux__

}