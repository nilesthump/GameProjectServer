#include "Util.h"
#include <windows.h>
namespace GameProjectServer
{
	uint32_t GetThreadId()
	{
		return GetCurrentThreadId();
	}

	uint32_t GetFiberId()
	{
#ifdef USE_FIBER
		return GetCurrentFiber();
#else
		return 0;
#endif
	}
}