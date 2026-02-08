#pragma once

#ifndef __Linux__
#include <windows.h>
#else
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#endif // !__Linux__


namespace GameProjectServer
{

#ifndef __Linux__
	DWORD getThreadId();
#else
	pid_t getThreadId();
#endif // !__Linux__


}