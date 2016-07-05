// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_NT_TIMEOUT_H_
#define __MCFCRT_ENV_NT_TIMEOUT_H_

#include "_crtdef.h"
#include "mcfwin.h"
#include "clocks.h"

_MCFCRT_EXTERN_C_BEGIN

static inline void __MCF_CRT_InitializeNtTimeout(LARGE_INTEGER *__pliTimeout, _MCFCRT_STD uint64_t __u64UntilFastMonoClock) _MCFCRT_NOEXCEPT {
	const _MCFCRT_STD uint64_t __u64Now = _MCFCRT_GetFastMonoClock();
	if(__u64UntilFastMonoClock < __u64Now){
		// We should time out immediately.
		__pliTimeout->QuadPart = 0;
		return;
	}
	const _MCFCRT_STD uint64_t __u64DeltaMs = __u64UntilFastMonoClock - __u64Now;
	if(__u64DeltaMs > INT64_MAX / 10000u - 1u){
		// We should never time out.
		__pliTimeout->QuadPart = INT64_MAX;
		return;
	}
	// If this value is negative, the duration is mensured by the absolute value of it, in 100 nanoseconds.
	// An increment of 9999u makes sure we never time out before the time point.
	__pliTimeout->QuadPart = -(_MCFCRT_STD int64_t)(__u64DeltaMs * 10000u + 9999u);
}

_MCFCRT_EXTERN_C_END

#endif
