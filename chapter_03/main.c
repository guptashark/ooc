#include <setjmp.h>

static enum tokens token;  /* current input symbol */

/* A buffer that saves the current environment so that 
 * when execution jumps back, all the registers, stack 
 * ptr and instruction pointer are all correct. 
 */
static jmp_buf onError;

int main(void) {

	/* Errors is volatile since its value may be 
	 * changed without a thread of execution 
	 * directly changing it. For instance, 
	 *
	 * errors = 0
	 * jump! 
	 * errors = 1
	 * jump back! 
	 * 
	volatile int errors = 0;

	/* Where we store the arithmetic expression */
	char buf[BUFSIZ];

	
	if(setjmp(onError)) {
		errors++;
	}

	whille(fgets(buf, BUFSIZ, stdin)) {
		if(scan(buf)) {
			void * e = sum();
			if(token) {
				error("trash after sum");
			}

			process(e);
			delete(e);
		}
	}

	return errors > 0;
}
