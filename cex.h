#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#ifdef _MSC_VER
#define inline __inline
#define bool int
#define true 1
#define false 0
#endif

//*************************************************
//* Structures ************************************
//*************************************************
typedef int cex_exception_type;

typedef struct _cex_exception {
	cex_exception_type type;
	char *what;
} cex_exception;

typedef struct _cex_frame {
	struct _cex_frame *previous;
	jmp_buf environment;
	cex_exception exception;
	char *exception_file;
	int exception_line;
	bool is_handled;
	bool is_finalized;
} cex_frame;

//*************************************************
//* Functions *************************************
//*************************************************
cex_frame *cex_frame_stack_top;

static inline cex_frame cex_frame_create() {
	cex_frame output;
	output.is_finalized = false;
	output.is_handled = false;
	output.exception.type = 0;

	return output;
}

static inline void cex_frame_push(cex_frame *frame) {
	frame->previous = cex_frame_stack_top;
	cex_frame_stack_top = frame;
}

static inline void cex_frame_pop() {
	cex_frame_stack_top = cex_frame_stack_top->previous;
}

static inline cex_exception cex_exception_create(cex_exception_type type, char *what) {
	cex_exception output;
	output.type = type;
	output.what = what;

	return output;
}

static inline void cex_exception_throw(cex_exception exception, char *file, int line) {
	if (cex_frame_stack_top == NULL) {
		fprintf(stderr, "Unhandled Exception: %s at %s, line %d\n", exception.what, file, line);
		fflush(stderr);
		abort();
	}

	cex_frame_stack_top->exception = exception;
	cex_frame_stack_top->exception_file = file;
	cex_frame_stack_top->exception_line = line;
	longjmp(cex_frame_stack_top->environment, exception.type);
}

//*************************************************
//* MACROS ****************************************
//*************************************************
#define THROW(type, what) cex_exception_throw(cex_exception_create(type, what), __FILE__, __LINE__)
#define EXCEPTION __frame.exception

#define TRY { \
	cex_frame __frame = cex_frame_create(); \
	int __result; \
	cex_frame_push(&__frame); \
	__result = setjmp(__frame.environment); \
	if (__result == 0) {

#define CATCH(ex) \
	} else if (__result == ex && !__frame.is_handled && !__frame.is_finalized) { \
		__frame.is_handled = true; \
		__result = 0;

#define FINALLY \
	} if (!__frame.is_finalized) { \
		__frame.is_finalized = true; \
		cex_frame_pop();

#define END_TRY \
	} if (!__frame.is_finalized) \
		cex_frame_pop(); \
	if (__result != 0) \
		cex_exception_throw(__frame.exception, __frame.exception_file, __frame.exception_line); \
}
