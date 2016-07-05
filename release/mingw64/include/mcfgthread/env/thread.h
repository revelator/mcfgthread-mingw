// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_THREAD_H_
#define __MCFCRT_ENV_THREAD_H_

#include "_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

typedef unsigned long (__attribute__((__stdcall__)) *_MCFCRT_NativeThreadProc)(void *__pParam);

typedef struct __MCFCRT_tagThreadHandle {
	int __n;
} *_MCFCRT_ThreadHandle;

extern _MCFCRT_ThreadHandle _MCFCRT_CreateNativeThread(_MCFCRT_NativeThreadProc __pfnThreadProc, void *__pParam, bool __bSuspended, _MCFCRT_STD uintptr_t *restrict __puThreadId) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_CloseThread(_MCFCRT_ThreadHandle __hThread) _MCFCRT_NOEXCEPT;

extern void _MCFCRT_Sleep(_MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT;
// _MCFCRT_AlertableSleep() returns true if the current thread has been woken up by APC and false if the current thread has timed out.
extern bool _MCFCRT_AlertableSleep(_MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_AlertableSleepForever(void) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_YieldThread(void) _MCFCRT_NOEXCEPT;

extern long _MCFCRT_SuspendThread(_MCFCRT_ThreadHandle __hThread) _MCFCRT_NOEXCEPT;
extern long _MCFCRT_ResumeThread(_MCFCRT_ThreadHandle __hThread) _MCFCRT_NOEXCEPT;

// _MCFCRT_WaitForThread() returns true if the other thread has terminated and false if the current thread has timed out.
extern bool _MCFCRT_WaitForThread(_MCFCRT_ThreadHandle __hThread, _MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_WaitForThreadForever(_MCFCRT_ThreadHandle __hThread) _MCFCRT_NOEXCEPT;

__attribute__((__const__))
extern _MCFCRT_STD uintptr_t _MCFCRT_GetCurrentThreadId(void) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif
