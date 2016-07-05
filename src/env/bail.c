// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "bail.h"
#include "mcfwin.h"
#include "../ext/wcpcpy.h"
#include <ntdef.h>
#if defined (__MINGW32__) && defined(__MINGW64_VERSION_MAJOR)
#include <ntstatus.h>
#else
#include <ddk/ntstatus.h>
#endif

typedef enum tagHardErrorResponseOption {
	kHardErrorAbortRetryIgnore,
	kHardErrorOk,
	kHardErrorOkCancel,
	kHardErrorRetryCancel,
	kHardErrorYesNo,
	kHardErrorYesNoCancel,
	kHardErrorShutdownSystem,
	kHardErrorTrayNotify,
	kHardErrorCancelTryAgainContinue,
} HardErrorResponseOption;

typedef enum tagHardErrorResponse {
	kHardErrorResponseReturnToCaller,
	kHardErrorResponseNotHandled,
	kHardErrorResponseAbort,
	kHardErrorResponseCancel,
	kHardErrorResponseIgnore,
	kHardErrorResponseNo,
	kHardErrorResponseOk,
	kHardErrorResponseRetry,
	kHardErrorResponseYes,
} HardErrorResponse;

__attribute__((__dllimport__, __stdcall__))
extern NTSTATUS NtRaiseHardError(NTSTATUS lStatus, DWORD dwParamCount, DWORD dwUnknown, const ULONG_PTR *pulParams, HardErrorResponseOption eOption, HardErrorResponse *peResponse)
	__asm__("NtRaiseHardError");

static volatile bool g_bBailed = false;

_Noreturn void _MCFCRT_Bail(const wchar_t *pwszDescription){
	const bool bBailed = __atomic_exchange_n(&g_bBailed, true, __ATOMIC_RELAXED);
	if(bBailed){
		TerminateThread(GetCurrentThread(), (DWORD)STATUS_UNSUCCESSFUL);
		__builtin_unreachable();
	}

#ifdef NDEBUG
	const bool bCanBeDebugged = IsDebuggerPresent();
#else
	const bool bCanBeDebugged = true;
#endif

	wchar_t awcBuffer[1024 + 128];
	wchar_t *pwcWrite = _MCFCRT_wcpcpy(awcBuffer, L"The program has asked MCF CRT to terminate it abnormally. Please contact the author for detailed information.");
	if(pwszDescription){
		pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L"\n\nError description: \n");
		pwcWrite = _MCFCRT_wcppcpy(pwcWrite, awcBuffer + 1024, pwszDescription); // Reserve some characters for the following sentence.
	}
	pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L"\n\nClick OK to terminate the program");
	if(bCanBeDebugged){
		pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L"; click Cancel to debug the program");
	}
	pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L".\n");

	const HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
	if(hStdErr != INVALID_HANDLE_VALUE){
		DWORD dwMode;
		if(GetConsoleMode(hStdErr, &dwMode)){
			DWORD dwCharsWritten;
			WriteConsoleW(hStdErr, awcBuffer, (DWORD)(pwcWrite - awcBuffer), &dwCharsWritten, nullptr);
		}
	}
	*(pwcWrite--) = 0;

	UNICODE_STRING ustrText;
	ustrText.Length        = (unsigned short)((char *)pwcWrite - (char *)awcBuffer);
	ustrText.MaximumLength = ustrText.Length;
	ustrText.Buffer        = awcBuffer;

	static const wchar_t kCaption[] = L"MCF CRT Error";
	UNICODE_STRING ustrCaption;
	ustrCaption.Length        = sizeof(kCaption) - sizeof(wchar_t);
	ustrCaption.MaximumLength = ustrCaption.Length;
	ustrCaption.Buffer        = (wchar_t *)kCaption;

	UINT uType = (bCanBeDebugged ? MB_OKCANCEL : MB_OK) | MB_ICONERROR;

	const ULONG_PTR aulParams[] = { (ULONG_PTR)&ustrText, (ULONG_PTR)&ustrCaption, uType, (ULONG_PTR)-1 };
	HardErrorResponse eResponse;
	const NTSTATUS lStatus = NtRaiseHardError(0x50000018, sizeof(aulParams) / sizeof(aulParams[0]), 3, aulParams, kHardErrorOk, &eResponse);
	if(!NT_SUCCESS(lStatus)){
		eResponse = kHardErrorResponseCancel;
	}
	if(eResponse != kHardErrorResponseOk){
#if defined (__MINGW32__) && defined(__MINGW64_VERSION_MAJOR)
		__debugbreak();
#else
		__builtin_trap();
#endif

	}
	TerminateProcess(GetCurrentProcess(), (DWORD)STATUS_UNSUCCESSFUL);
	__builtin_unreachable();
}
