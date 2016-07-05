// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_ONCE_FLAG_H_
#define __MCFCRT_ENV_ONCE_FLAG_H_

#include "_crtdef.h"

#ifndef __MCFCRT_INLINE_OR_EXTERN
#	define __MCFCRT_INLINE_OR_EXTERN     __attribute__((__gnu_inline__)) extern inline
#endif

_MCFCRT_EXTERN_C_BEGIN

// In the case of static initialization, please initialize it with { 0 }.
typedef struct __MCFCRT_tagOnceFlag {
	volatile _MCFCRT_STD uintptr_t __u;
} _MCFCRT_OnceFlag;

typedef enum __MCFCRT_tagOnceResult {
	_MCFCRT_kOnceResultTimedOut = 1,
	_MCFCRT_kOnceResultInitial  = 2,
	_MCFCRT_kOnceResultFinished = 3,
} _MCFCRT_OnceResult;

__MCFCRT_INLINE_OR_EXTERN void _MCFCRT_InitializeOnceFlag(_MCFCRT_OnceFlag *__pOnceFlag) _MCFCRT_NOEXCEPT {
	__atomic_store_n(&(__pOnceFlag->__u), 0, __ATOMIC_RELEASE);
}

extern _MCFCRT_OnceResult _MCFCRT_WaitForOnceFlag(_MCFCRT_OnceFlag *__pOnceFlag, _MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT;
extern _MCFCRT_OnceResult _MCFCRT_WaitForOnceFlagForever(_MCFCRT_OnceFlag *__pOnceFlag) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_SignalOnceFlagAsFinished(_MCFCRT_OnceFlag *__pOnceFlag) _MCFCRT_NOEXCEPT;
extern void _MCFCRT_SignalOnceFlagAsAborted(_MCFCRT_OnceFlag *__pOnceFlag) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif
