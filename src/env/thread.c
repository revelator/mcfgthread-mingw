// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "thread.h"
#include "mcfwin.h"
#include "fenv.h"
#include "_nt_timeout.h"
#include "../ext/assert.h"
#if defined (__MINGW32__) && defined(__MINGW64_VERSION_MAJOR)
#include <winternl.h>
#else
#include <ddk/ntstatus.h>
#endif
#include <ntdef.h>

#if defined (__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NTAPI NtClose(HANDLE Handle)	
	__asm__("NtClose");
__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NTAPI NtWaitForSingleObject(HANDLE Handle,BOOLEAN Alertable,PLARGE_INTEGER Timeout)	
	__asm__("NtWaitForSingleObject");
#endif

__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NtDelayExecution(BOOLEAN bAlertable, const LARGE_INTEGER *pliTimeout)
	__asm__("NtDelayExecution");
__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NtYieldExecution(void)
	__asm__("NtYieldExecution");

__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NtSuspendThread(HANDLE hThread, LONG *plPrevCount)
	__asm__("NtSuspendThread");
__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NtResumeThread(HANDLE hThread, LONG *plPrevCount)
	__asm__("NtResumeThread");

_MCFCRT_ThreadHandle _MCFCRT_CreateNativeThread(_MCFCRT_NativeThreadProc pfnThreadProc, void *pParam, bool bSuspended, uintptr_t *restrict puThreadId){
	DWORD dwThreadId;
	const HANDLE hThread = CreateRemoteThread(GetCurrentProcess(), nullptr, 0, pfnThreadProc, pParam, bSuspended ? CREATE_SUSPENDED : 0, &dwThreadId);
	if(!hThread){
		return nullptr;
	}
	if(puThreadId){
		*puThreadId = dwThreadId;
	}
	return (_MCFCRT_ThreadHandle)hThread;
}
void _MCFCRT_CloseThread(_MCFCRT_ThreadHandle hThread){
	const NTSTATUS lStatus = NtClose((HANDLE)hThread);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtClose() failed.");
}

void _MCFCRT_Sleep(uint64_t u64UntilFastMonoClock){
	LARGE_INTEGER liTimeout;
	__MCF_CRT_InitializeNtTimeout(&liTimeout, u64UntilFastMonoClock);
	const NTSTATUS lStatus = NtDelayExecution(false, &liTimeout);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtDelayExecution() failed.");
}
bool _MCFCRT_AlertableSleep(uint64_t u64UntilFastMonoClock){
	LARGE_INTEGER liTimeout;
	__MCF_CRT_InitializeNtTimeout(&liTimeout, u64UntilFastMonoClock);
	const NTSTATUS lStatus = NtDelayExecution(true, &liTimeout);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtDelayExecution() failed.");
	if(lStatus == STATUS_TIMEOUT){
		return false;
	}
	return true;
}
void _MCFCRT_AlertableSleepForever(void){
	LARGE_INTEGER liTimeout;
	liTimeout.QuadPart = INT64_MAX;
	const NTSTATUS lStatus = NtDelayExecution(true, &liTimeout);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtDelayExecution() failed.");
}
void _MCFCRT_YieldThread(void){
	const NTSTATUS lStatus = NtYieldExecution();
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtYieldExecution() failed.");
}

long _MCFCRT_SuspendThread(_MCFCRT_ThreadHandle hThread){
	LONG lPrevCount;
	const NTSTATUS lStatus = NtSuspendThread((HANDLE)hThread, &lPrevCount);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtSuspendThread() failed.");
	return lPrevCount;
}
long _MCFCRT_ResumeThread(_MCFCRT_ThreadHandle hThread){
	LONG lPrevCount;
	const NTSTATUS lStatus = NtResumeThread((HANDLE)hThread, &lPrevCount);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtResumeThread() failed.");
	return lPrevCount;
}

bool _MCFCRT_WaitForThread(_MCFCRT_ThreadHandle hThread, uint64_t u64UntilFastMonoClock){
	LARGE_INTEGER liTimeout;
	__MCF_CRT_InitializeNtTimeout(&liTimeout, u64UntilFastMonoClock);
	const NTSTATUS lStatus = NtWaitForSingleObject((HANDLE)hThread, false, &liTimeout);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtWaitForSingleObject() failed.");
	if(lStatus == STATUS_TIMEOUT){
		return false;
	}
	return true;
}
void _MCFCRT_WaitForThreadForever(_MCFCRT_ThreadHandle hThread){
	const NTSTATUS lStatus = NtWaitForSingleObject((HANDLE)hThread, false, nullptr);
	_MCFCRT_ASSERT_MSG(NT_SUCCESS(lStatus), L"NtWaitForSingleObject() failed.");
}

uintptr_t _MCFCRT_GetCurrentThreadId(void){
	return GetCurrentThreadId();
}
