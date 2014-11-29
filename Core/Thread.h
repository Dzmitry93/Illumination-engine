#pragma once
#ifndef __THREAD_H__
#define __THREAD_H__

#include <Windows.h>
#include "Debug.h"

namespace OGE
{
	class Thread
	{
	public:
		Thread(void* pPtr, void* pParams);
		~Thread(void);

		void suspend();
		void resume();

		void stop();

	private:
		void* pFunc;
		void* pParams;

		LPDWORD pThreadID;

		HANDLE threadHandle;
	};

	inline Thread::Thread(void* pFunc, void* pParams) : pFunc(pFunc), pParams(pParams), pThreadID(NULL)
	{	
		threadHandle = CreateThread(NULL, 1 << 20, (LPTHREAD_START_ROUTINE) pFunc, pParams, CREATE_SUSPENDED, pThreadID);
		debug_assert(threadHandle != nullptr, "cannot create thread");
	}

	inline Thread::~Thread()
	{
		ExitThread(0);
	}

	inline void Thread::resume()
	{
		DWORD result = ResumeThread(threadHandle);
		debug_assert(result > -1, "cannot resume thread");
	}

	inline void Thread::suspend()
	{
		DWORD result = SuspendThread(threadHandle);
		debug_assert(result > -1, "cannot suspend thread");
	}

	inline void stop() 
	{
		ExitThread(0);
	}
}

#endif
