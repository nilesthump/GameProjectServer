#include "Util.h"

namespace GameProjectServer
{

#ifndef __Linux__
	uint32_t GetThreadId()
	{
		return GetCurrentThreadId();
	}
#else
	pid_t GetThreadId()
	{
		return syscall(SYS_gettid);
	}
#endif // !__Linux__

}