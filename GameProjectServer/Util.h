#pragma once

#ifndef __Linux__
#include <windows.h>
#include <cstdint>
#else
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#endif // !__Linux__


namespace GameProjectServer
{

#ifndef __Linux__
	uint32_t GetThreadId();
#else
	pid_t GetThreadId();
#endif // !__Linux__


}