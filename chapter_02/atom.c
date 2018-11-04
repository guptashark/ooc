#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "atom.h"
#include "new.h"
#include "new.r"

/* the textbook reuses the string name, but this means
 * that it is not possible to work with both strings
 * and atoms in the main file. Instetad, we will 
 * use the name 'atom', and then in the main file 
 * simply change the type of the objects being 
 * constructed - to atom instead of string. 
 */
struct atom {
	const void * class;

	char * text;
	struct atom *next;
	unsigned count;
};

/* The internal counter of all strings */
static struct atom * ring;

/* what... is this. Isn't this bad? 
 * I am unclear on wether the ptr that 
 * the fn is returning is safe to use. 
 * It has the same value in terms of what
 * it points to, but it has a different 
 * address. The only thing I can imagine 
 * is that it doesn't matter since when we 
 * return, we're not copying the address over, 
 * we're only copying the ptr value over, which 
 * would be going into a ptr in the main fn
 * anyways
 */
static void * atom_clone(const void * self_arg) {
	struct atom * self = (void *)self_arg;
	self->count++;
	return self;
}


/* if the ring is null, return 0, 
 * if the ring contains the text, return 1, 
 * 	and write the ptr into 'original'
 * if the ring doesn't contain the text, return -1. 
 */
static int 
ring_check(const char *text, struct atom ** ap) {
	struct atom *p;

	if(ring == NULL) {
		return 0;
	}

	p = ring;
	do {
		if(strcmp(p->text, text) == 0) {
			*ap = p;
			return 1;
		}

		p = p->next;
	} while (p != ring);

	return -1;
}


	

static void * atom_ctor(void * self_arg, va_list *app) {
	struct atom *p = NULL;
	int ret = 0;
	struct atom * self = self_arg;
	const char * text = va_arg(*app, const char *);

	ret = ring_check(text, &p);

	if(ret == 0) {
		ring = self;
	} else if(ret == 1) {
		p->count++;
		free(self);
		return p;
	} else {
		self-> next = ring->next;
		ring->next = self;
		self->count = 1;
	}
	
	self->text = malloc(strlen(text) + 1);
	assert(self->text);
	strcpy(self->text, text);
	return self;
}



/* check to see if the text is the only copy left. 
 * if so, then we delete it from the ring. Otherwise, 
 * we don't delete anything, instead decrementing 
 * the count by 1. return a null since we don't 
 * want to be deletig anything. 
 */
static void * atom_dtor(void * self_arg) {
	struct atom * p = self_arg;

	/* it's not possible for this atom 
	 * to not be in the ring. Get the count. 
	 */

	if (p->count > 1) {
		p->count--;
		printf("%u copies left.\n", p->count);
		return NULL;
	} else {
		printf("deleting last copy (unimplemented)\n");
		/* need to delete this item 
		 * from the ring since it's the 
		 * last of its kind
		 */

		/* free(p->text); */

		/* how do we get the atom that 
		 * points to this one? 
		 */
		return NULL;
	}
}
		
		
const void * atom;



