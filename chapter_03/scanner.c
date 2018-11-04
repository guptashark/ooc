/* ctype.h is functions for 
 * determining character type. 
 * $ man ctype.h
 * will give the posix manual
 * for all the fn's in it. 
 *
 */
#include <ctype.h>
/* Defines the errno global 
 * variable/macro that is 
 * used to effectively 
 * communicate the errors 
 * between calling processes. 
 */
#include <errno.h>
#include <stdlib.h>

/* Defines NUMBER */
#include "parse.h"

static double number; 

/* the function that scan's the buffer
 * to produce the next token that 
 * can then be parsed into an arithmetic
 * expression. 
 */
static enum tokens 
scan(const char *buf) {

	/* This is static because it 
	 * needs to be a the right 
	 * position to process the 
	 * next token in buf. This means
	 * that the function is only 
	 * ever called with a non-null
	 * buff when called with new input.
	 */
	static const char * bp;
	if(buf) {
		bp = buf;	
	}

	/* We don't process spaces */
	while(isspace(*bp)) {
		bp++;
	}

	/* digit or decimal */
	if(isdigit(*bp) || *bp == '.') {
		/* clear errno */
		errno = 0;
		token = NUMBER;
		/* number is a global var 
		 * in this file  - makes it
		 * easy to pass to other fn's. 
		 *
		 * Cast away the const so that 
		 * the function doesn't hit a 
		 * compile time error.  
		 */
		number = strtod(bp, (char **) &bp);

		/* How would this ever happen? isn't 
		 * *bp guaranteed to be a digit or period? 
		 */
		if(errno == ERANGE) {
			/* passing ERANGE to our error fn */
			error("bad value: %s", strerror(errno));
		}
	} else {
		/* Check to see if derefing
		 * bp gives a non-null. If yes, 
		 * then token is that value, 
		 * and bp is incremented. 
		 * 
		 * This expression is confusing: 
		 * since first bp is returned, 
		 * which is then dereferenced, 
		 * but then incremented to the 
		 * next. So all is good. 
		 * But if it was *++bp, then 
		 * the next ptr would be 
		 * dereferenced, since 
		 * ++i increments then returns, 
		 * whereas i++ returns then 
		 * increments. 
		 *
		 * TODO Learn more about this. 
		 *
		 * Obviously if *bp doesn't 
		 * exist... what even is that?? 
		 * perhaps a null character in 
		 * the buffer, ie we've reached
		 * the end then 0 is the token.
		 */
		token = *bp ? *bp++: 0;
	}

	return token;
}



