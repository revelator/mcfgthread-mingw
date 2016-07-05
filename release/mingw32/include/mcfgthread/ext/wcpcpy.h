// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#ifndef __MCFCRT_EXT_WCPCPY_H_
#define __MCFCRT_EXT_WCPCPY_H_

#include "../env/_crtdef.h"

_MCFCRT_EXTERN_C_BEGIN

extern wchar_t *_MCFCRT_wcpcpy(wchar_t *restrict __dst, const wchar_t *restrict __src) _MCFCRT_NOEXCEPT;
extern wchar_t *_MCFCRT_wcppcpy(wchar_t *__dst, wchar_t *__end, const wchar_t *restrict __src) _MCFCRT_NOEXCEPT;

_MCFCRT_EXTERN_C_END

#endif
