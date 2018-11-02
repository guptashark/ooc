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


int differ(const void * self, const void * b) {
	/* What is this type decl... jesus */
	/* const struct class * const * cp = self */
	/* A pointer to a constant pointer to a 
	 * const struct. Seems unnecessary. 
	 */

	/* For now, operate under the assumption 
	 * that if self is a valid ptr (passes assert)
	 * then it points to a valid type, and is safe to 
	 * dereference. 
	 *
	 * book addresses this problem by saying there
	 * are several solutions to ensure self is 
	 * a valid obj ptr: 
	 * 	- compre *self to address range of valid 
	 * 	types
	 * 	- put a magic number at the beginning 
	 * 	of each type description. 
	 * 	- wait until chapter 8. 
	 */

	/* Instead of using the double ptr, 
	 * we'll use the single. 
	 */
	const struct class * c;
	assert(self);	
	c = *(const struct class **)self;

	assert(c && c->differ);
	return c->differ(self, b);
}


/* An example of a polymorphic function that 
 * doesn't use dynamic linkage. No other fn is 
 * called, or linked in at execution time. 
 * The fn is still polymorphic because it can 
 * operate on a variety of objects. 
 *
 * For instance, the "+" fn is polymorphic in 
 * C becuase it can work on both ints and float. 
 *
 * We implement our own size_of because C's sizeof 
 * only works at compile time. If it we were to 
 * apply it to any of our objects, we would only 
 * ever get the size of a void *. We have to write
 * our own to get the size that was allocated to it
 * by new.
 */
size_t size_of(const void * self) {
	assert(self);
	const struct class * c;
	c = *(const struct class **)self;

	assert(c);
	return c->size;
}
