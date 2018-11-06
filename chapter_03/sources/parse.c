#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"	/* defines NUMBER */
#include "value.h"

/*
 *	scanner
 */

/* current input symbol */
static enum tokens token;

/* if NUMBER: numerical value */
static double number;		

static enum tokens 
scan (const char * buf)
	/* return token = next input symbol */
{	static const char * bp;

	/* new input line */

	if (buf)
		bp = buf;			
	while (isspace(* bp & 0xff))
		++ bp;
	if (isdigit(* bp & 0xff) || * bp == '.')
	{	errno = 0;
		token = NUMBER;
		/* strtod takes in as many chars 
		 * as it can to suitably convert
		 * the string of digits into a 
		 * floating point number. It stops
		 * at the first non digit char, 
		 * and then sets bp as a ptr to 
		 * that char. if the numebr is
		 * too big or too small, it 
		 * returns ERANGE. 
		 *
		 */
	       	number = strtod(bp, (char **) & bp);
		if (errno == ERANGE)
			error("bad val: %s", strerror(errno));
	}
	else
		token = * bp ? * bp ++ : 0;
	return token;
}

/*
 *	factor : + factor
 *			 - factor
 *			 NUMBER
 *			 ( sum )
 */

static void * sum (void);

static void * factor (void)
{	void * result;

	switch ((int)token) {
	case '+':
		scan(0);
		return factor();
	case '-':
		scan(0);
		/* Note here that we're 
		 * returning Minus, not 
		 * Sub. Minus is a unary
		 * op. 
		 */
		return new(Minus, factor());
	case 'i':
		scan(0);
		return new(Inverse, factor());
	case 's':
		scan(0);
		return new(Sine, factor());
	/* Isn't this bad? If the token is 
	 * neither + or -, then it'll default, 
	 * when it actually could be a NUMBER
	 * or a left paren?? 
	 * figure this out:
	 *
	 * Answer: 
	 * The program steps through and doesn't 
	 * care where default is - it is only ever
	 * called once all cases are exhausted. 
	 */
	default:
		error("bad factor: '%c' 0x%x", token, token);
	case NUMBER:
		result = new(Value, number);
		break;
	case '(':
		scan(0);
		result = sum();
		if (token != ')')
			error("expecting )");
	}
	scan(0);
	return result;
}

/*
 *	product : factor { *|/ factor }...
 */

static void * product (void)
{	void * result = factor();
	const void * type;

	for (;;)
	{	switch ((int)token) {
		case '*':
			type = Mult;
			break;
		case '/':
			type = Div;
			break;
		case '%':
			type = Remainder;
			break;
		default:
			return result;
		}
		scan(0);
		result = new(type, result, factor());
	}
}

/*
 *	sum : product { +|- product }...
 */

static void * sum (void)
{	void * result = product();
	const void * type;

	for (;;)
	{	switch ((int)token) {
		case '+':
			type = Add;
			break;
		case '-':
			type = Sub;
			break;
		default:
			return result;
		}
		scan(0);
		result = new(type, result, product());
	}
}

/*
 *	sum \n ...
 */

static jmp_buf onError;

int main (void)
{	volatile int errors = 0;
	char buf [BUFSIZ];

	if (setjmp(onError))
		++ errors;

	while (fgets(buf, sizeof buf, stdin))
		if (scan(buf))
		{	void * e = sum();

			if (token)
				error("trash after sum");
			process(e);
			delete(e);
		}

	return errors > 0;
}

void error (const char * fmt, ...)
{	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap), putc('\n', stderr);
	va_end(ap);
	longjmp(onError, 1);
}
