// This file is put into the Public Domain.

#include "../src/env/gthread.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

// tls
__gthread_key_t key;

void tls_destructor(void *p){
	printf("destructing tls data %u\n", *(unsigned *)p);
	free(p);
}

// mutex
__gthread_mutex_t mutex = { 0 };
volatile unsigned long counter = 0;

#define INCREMENT_PER_THREAD   1000000ul
#define THREAD_COUNT           4ul

__MCFCRT_C_STDCALL
unsigned long test_thread_proc(void * param){
	unsigned *p = malloc(sizeof(unsigned));
	assert(p);
	*p = (unsigned)(uintptr_t)param;
	int ret = __gthread_setspecific(key, p);
	assert(ret == 0);
	printf("constructed tls data %u\n", *(unsigned *)p);

	for(unsigned long i = 0; i < INCREMENT_PER_THREAD; ++i){
		__gthread_mutex_lock(&mutex);
		unsigned c = counter;
		++c;
		counter = c;
		__gthread_mutex_unlock(&mutex);
	}

	return 0;
}

int main(){
	int ret = __gthread_key_create(&key, &tls_destructor);
	assert(ret == 0);
	printf("key = %p\n", (void *)key);

	_MCFCRT_ThreadHandle threads[THREAD_COUNT];
	for(unsigned i = 0; i < THREAD_COUNT; ++i){
		_MCFCRT_ThreadHandle h = _MCFCRT_CreateNativeThread(&test_thread_proc, (void *)(intptr_t)i, false, nullptr);
		assert(h);
		threads[i] = h;
	}
	for(unsigned i = 0; i < THREAD_COUNT; ++i){
		_MCFCRT_ThreadHandle h = threads[i];
		printf("waiting for thread %u\n", i);
		_MCFCRT_WaitForThreadForever(h);
		_MCFCRT_CloseThread(h);
	}
	printf("counter = %lu\n", counter);

	__gthread_key_delete(key);

	assert(counter == INCREMENT_PER_THREAD * THREAD_COUNT);
}
