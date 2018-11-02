#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "new.h"

struct class {
	/* new needs to know how much mem to allocate */
	size_t size;

	/* new calls this to initialize obj
	 * (may install dynamic mem for itself) */
	void * (* ctor) (void * self, va_list *app);
	/* delete calls this to properly destroy 
	 * the obj, with dynamically allocated mem */
	void * (* dtor) (void * self);
	void * (* clone) (const void * self);
	/* Not a simple case of pointer difference 
	 * anymore. objects could be math style
	 * matrices and have same numbers, but are 
	 * different objects (copies). Like 
	 * python deepcopy, I think */
	int (* differ)(const void * self, const void * b);
};

void * new (const void * class_arg, ... ) {
	const struct class * c = class_arg;
	void * p = calloc(1, c-> size);

	assert(p);

	/* after allocating mem for the 
	 * obj based on its type, we can 
	 * write in the type to the first 
	 * address of the obj. C guarantees
	 * that you can get the first element 
	 * of a struct by dereferencing a ptr 
	 * to it. (ie, a struct never has 
	 * padding before first field.
	 *
	 * (p is a ptr to an obj. An obj's 
	 * first field must be a ptr to its
	 * class.)
	 */
	*(const struct class **)p = c;

	/* Not necessary for the obj to have a 
	 * ctor, it might be fine with a zeroed
	 * out struct. A change would be to 
	 * force each object to have a ctor, 
	 * which is the default ctor, and then 
	 * manually zeroes everything out, 
	 * and we call malloc instead of 
	 * calloc to allocate memory. 
	 */
	if(c -> ctor) {
		va_list ap;

		/* Pass along ctor args 
		 * to the obj for construction. 
		 * An issue here is how would 
		 * one write multiple constructors? 
		 * How would the ctor know? It would 
		 * have to parse a format string to 
		 * determine how many args, and what
		 * type they are... 
		 */
		va_start(ap, class_arg);
		p = c -> ctor(p, &ap);
		va_end(ap);
	}

	return p;
}



void delete(void * self) {
	const struct class * c;
	assert(self);

	c = *(const struct class **)self;
	if(c && c->dtor) {
		self = c->dtor(self);
	}

	/* This counts on the dtor returning 
	 * the pointer it was fed. Unsure why 
	 * we have it assign to itself, why 
	 * not simply delete self without 
	 * reassining it? dtor could cheat
	 * and return a null pointer, ensuring
	 * the object isn't deleted. 
	 *
	 *TODO Why is this the way it is? 
	 */ 
	free(self);
}
