#include "Util.h"
#include <windows.h>
namespace GameProjectServer
{
	uint32_t GetThreadId()
	{
		return GetCurrentThreadId();
	}
}