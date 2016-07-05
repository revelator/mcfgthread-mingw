// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "clocks.h"
#include "mcfwin.h"
#include "bail.h"

#if defined (__MINGW32__) && !defined(__MINGW64_VERSION_MAJOR)
typedef struct _TIME_DYNAMIC_ZONE_INFORMATION {
	LONG Bias;
	WCHAR StandardName[32];
	SYSTEMTIME StandardDate;
	LONG StandardBias;
	WCHAR DaylightName[32];
	SYSTEMTIME DaylightDate;
	LONG DaylightBias;
	WCHAR TimeZoneKeyName[128];
	BOOLEAN DynamicDaylightTimeDisabled;
} DYNAMIC_TIME_ZONE_INFORMATION,*PDYNAMIC_TIME_ZONE_INFORMATION;

WINBASEAPI DWORD WINAPI GetDynamicTimeZoneInformation (PDYNAMIC_TIME_ZONE_INFORMATION pTimeZoneInformation);
WINBASEAPI ULONGLONG WINAPI GetTickCount64 (VOID);
#endif

static uint64_t GetTimeZoneOffsetInMillisecondsOnce(void){
	static uint64_t *volatile s_pu64Inited;
	static uint64_t           s_u64Value;

	uint64_t *pInited = __atomic_load_n(&s_pu64Inited, __ATOMIC_CONSUME);
	if(!pInited){
		pInited = &s_u64Value;

		DYNAMIC_TIME_ZONE_INFORMATION vInfo;
		if(GetDynamicTimeZoneInformation(&vInfo) == TIME_ZONE_ID_INVALID){
			_MCFCRT_Bail(L"GetDynamicTimeZoneInformation() failed.");
		}
		*pInited = (uint64_t)(vInfo.Bias * -60000ll);

		__atomic_store_n(&s_pu64Inited, pInited, __ATOMIC_RELEASE);
	}
	return *pInited;
}
static double QueryPerformanceFrequencyReciprocalOnce(void){
	static double *volatile s_plfInited;
	static double           s_ulfValue;

	double *pInited = __atomic_load_n(&s_plfInited, __ATOMIC_CONSUME);
	if(!pInited){
		pInited = &s_ulfValue;

		LARGE_INTEGER liFreq;
		if(!QueryPerformanceFrequency(&liFreq)){
			_MCFCRT_Bail(L"QueryPerformanceFrequency() failed.");
		}
		*pInited = 1000.0 / (double)liFreq.QuadPart;

		__atomic_store_n(&s_plfInited, pInited, __ATOMIC_RELEASE);
	}
	return *pInited;
}

uint64_t _MCFCRT_GetUtcClock(void){
	union {
		FILETIME ft;
		LARGE_INTEGER li;
	} unUtc;
	GetSystemTimeAsFileTime(&unUtc.ft);
	// 0x019DB1DED53E8000 = duration since 1601-01-01 until 1970-01-01 in nanoseconds.
	return (uint64_t)(((double)unUtc.li.QuadPart - 0x019DB1DED53E8000ll) / 10000.0);
}
uint64_t _MCFCRT_GetLocalClock(void){
	return _MCFCRT_GetLocalClockFromUtc(_MCFCRT_GetUtcClock());
}

uint64_t _MCFCRT_GetUtcClockFromLocal(uint64_t u64LocalClock){
	return u64LocalClock - GetTimeZoneOffsetInMillisecondsOnce();
}
uint64_t _MCFCRT_GetLocalClockFromUtc(uint64_t u64UtcClock){
	return u64UtcClock + GetTimeZoneOffsetInMillisecondsOnce();
}

uint64_t _MCFCRT_GetFastMonoClock(void){
	return GetTickCount64();
}
double _MCFCRT_GetHiResMonoClock(void){
	LARGE_INTEGER liCounter;
	if(!QueryPerformanceCounter(&liCounter)){
		_MCFCRT_Bail(L"QueryPerformanceCounter() failed.");
	}
	return (double)liCounter.QuadPart * QueryPerformanceFrequencyReciprocalOnce();
}
