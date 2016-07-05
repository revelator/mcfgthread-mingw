// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "env/mcfwin.h"
#include "env/thread_env.h"
#include "env/_seh_top.h"

__MCFCRT_C_STDCALL
BOOL __MCFCRT_DllStartup(HINSTANCE hDll, DWORD dwReason, LPVOID pReserved)
	__asm__("@__MCFCRT_DllStartup");

static bool g_bInitialized = false;

__MCFCRT_C_STDCALL
BOOL __MCFCRT_DllStartup(HINSTANCE hDll, DWORD dwReason, LPVOID pReserved){
	(void)hDll;
	(void)pReserved;

	bool bRet = true;

	__MCFCRT_SEH_TOP_BEGIN
	{
		switch(dwReason){
		case DLL_PROCESS_ATTACH:
			if(g_bInitialized){
				break;
			}
			bRet = __MCFCRT_ThreadEnvInit();
			if(!bRet){
				break;
			}
			g_bInitialized = true;
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			__MCFCRT_TlsCleanup();
			break;

		case DLL_PROCESS_DETACH:
			if(!g_bInitialized){
				break;
			}
			g_bInitialized = false;
			__MCFCRT_TlsCleanup();
			__MCFCRT_ThreadEnvUninit();
			break;
		}
	}
	__MCFCRT_SEH_TOP_END

	return bRet;
}
