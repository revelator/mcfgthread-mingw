// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "assert.h"
#include "../ext/wcpcpy.h"
#include "../ext/itow.h"
#include "../env/bail.h"

__attribute__((__noreturn__))
void __MCFCRT_OnAssertionFailure(const wchar_t *pwszExpression, const wchar_t *pwszFile, unsigned long ulLine, const wchar_t *pwszMessage){
	wchar_t awcBuffer[1024];
	wchar_t *pwcWrite = _MCFCRT_wcpcpy(awcBuffer, L"Assertion failed!\n\nExpression: ");
	pwcWrite = _MCFCRT_wcppcpy(pwcWrite, awcBuffer + 256, pwszExpression); // Truncate the expression if it is too long.
	pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L"\nFile: ");
	pwcWrite = _MCFCRT_wcppcpy(pwcWrite, awcBuffer + 512, pwszFile);       // Do the same thing to the file name.
	pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L"\nLine: ");
	pwcWrite = _MCFCRT_itow_u(pwcWrite, ulLine);
	pwcWrite = _MCFCRT_wcpcpy(pwcWrite, L"\nDesc: ");
	pwcWrite = _MCFCRT_wcppcpy(pwcWrite, awcBuffer + 1024, pwszMessage);
	*pwcWrite = 0;
	_MCFCRT_Bail(awcBuffer);
}
