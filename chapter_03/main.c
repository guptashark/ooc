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
	 * execution assumes errors is 0 -> bad! 
	 * volatile forces reload on every occassion: 
	 * 	-> telling compiler that the value 
	 * 	may change without this thread of 
	 * 	executions knowledge
	 */
	volatile int errors = 0;

	/* Where we store the arithmetic expression */
	char buf[BUFSIZ];


	/* an error counter. setjmp returns zero 
	 * initially. However, when longjmp 
	 * returns, it returns to this location, 
	 * and then setjmp returns whatever is 
	 * in the longjmp arg. So, right now
	 * there are zero errors, but if longjmp
	 * finds one, it'll return as such, and 
	 * we'll then add that to the errs counter, 
	 */	
	if(setjmp(onError)) {
		errors++;
	}


	while(fgets(buf, BUFSIZ, stdin)) {
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

/* takes in a format string with varargs, 
 * initialize ptr to varargs
 * use printf but to stderr, pass along varargs
 * long jump with an error for errors counter
 *
 */
void error(const char * fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
       	putc('\n', stderr);
	va_end(ap);
	longjmp(onError, 1);
}


