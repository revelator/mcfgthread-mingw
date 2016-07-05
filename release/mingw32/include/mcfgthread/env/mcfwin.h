// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_ENV_MCFWIN_H_
#define __MCFCRT_ENV_MCFWIN_H_

#include "_crtdef.h"

#undef WINVER
#undef _WIN32_WINNT
#undef WIN32_LEAN_AND_MEAN

#define WINVER                      0x0601
#define _WIN32_WINNT                0x0601
#define WIN32_LEAN_AND_MEAN         1

#undef CDECL
#undef CALLBACK
#undef WINAPI
#undef WINAPIV
#undef APIENTRY

#define CDECL                       __MCFCRT_C_CDECL
#define CALLBACK                    __MCFCRT_C_STDCALL
#define WINAPI                      __MCFCRT_C_STDCALL
#define WINAPIV                     __MCFCRT_C_CDECL
#define APIENTRY                    __MCFCRT_C_STDCALL

#include <windows.h>

#endif
