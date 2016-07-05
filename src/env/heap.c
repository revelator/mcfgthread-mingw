// This file is part of MCFCRT.
// See MCFLicense.txt for licensing information.
// Copyleft 2013 - 2016, LH_Mouse. All wrongs reserved.

#include "heap.h"
#include "mcfwin.h"

unsigned char *__MCFCRT_HeapAlloc(size_t uSize, bool bFillsWithZero, const void *pRetAddr){
	(void)pRetAddr;

	return HeapAlloc(GetProcessHeap(), bFillsWithZero ? HEAP_ZERO_MEMORY : 0, uSize);
}
unsigned char *__MCFCRT_HeapRealloc(void *pBlock, size_t uSize, bool bFillsWithZero, const void *pRetAddr){
	(void)pRetAddr;

	return HeapReAlloc(GetProcessHeap(), bFillsWithZero ? HEAP_ZERO_MEMORY : 0, pBlock, uSize);
}
void __MCFCRT_HeapFree(void *pBlock, const void *pRetAddr){
	(void)pRetAddr;

	HeapFree(GetProcessHeap(), 0, pBlock);
}

