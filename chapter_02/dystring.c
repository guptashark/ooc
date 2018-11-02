#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct string {
	const void * class;
	char * text;
};

/* static because this function should not be visible
 * anywhere. The only way that new can call this fn 
 * is by knowing that there is an fn at the address
 * provided by the type. No other file ever calls
 * this directly, they can only call it indirectly. 
 *
 * Kinda shaky on the semantics there though, in 
 * terms of visibility and linkage. 
 *
 * TODO Understand the static linkage for functions
 * and how it works. Becuase my previous understanding
 * was that only functions that are not static in 
 * this c file can call this fn. But if it can 
 * be called indirectly from elsewhere... then... 
 * what part is really visible and enforced? 
 */
static void * string_ctor(void * self_arg, va_list *app) {
	struct string * s = self_arg;
	const char * text = va_arg(*app, const char *);

	s->text = malloc(strlen(text) + 1);
	strcpy(s -> text, text);
	return s;
}


static void * string_dtor(void * self_arg) {
	struct string * s = self_arg;
	free(s->text);
	s->text = NULL;
	return s;
}

