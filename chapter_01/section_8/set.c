#include <assert.h>
#include <stdlib.h>

/* This is a different implementation for
 * Set. Since we don't want to copy all
 * of the files over, we separate this 
 * different implementation into its own
 * folder, and then can compile from it
 * as needed, in the case where we use
 * this impelementation as opposed to the 
 * one made before
 */

/* struct set and set are two differen things. 
 * struct set is in the struct namespace, 
 * and refers to the actual iplementation, 
 * while set is a global id with type void *, 
 * and is a type. Users can use this type if
 * they decide to include the header.
 */
struct set { unsigned count; };
struct object { unsigned count; struct set * in; };

/* We need these so that the new fn knows 
 * how much memory to allocate for the 
 * object based on its type
 */
static const size_t set_size = sizeof(struct set);
static const size_t object_size = sizeof(struct object);

const void * set = &set_size;
const void * object = &object_size;


/* NEW_H implementation */

/* Now, new is different, it allocates right 
 * amount of memory. Of course, it has to 
 * cast the void ptr to the right type to 
 * do this, but this is fine, because we 
 * know what the void ptr actually points to.
 */
void * new (const void * type, ...) {
	const size_t size = *(const size_t *)type;
	void * p = calloc(1, size);

	assert(p);
	return p;
}

void delete(void * item) {
	free(item);
}

/* SET_H implementation */
void * add(void * set_arg, const void * element_arg) {
	/* since we're not using the set adt type, 
	 * we can use the local variable set. But, 
	 * this level of overloading is getting bad - 
	 * we already have a struct set, a global 
	 * id set, and now a local variable set. 
	 * We'll strive to instead use the first 
	 * letter of the type for local id's, if 
	 * the id conflicts with the global type name
	 */
	struct set * s = set_arg; 
	struct object * element = (void *)element_arg;

	assert(s);
	assert(element);

	if(!element -> in ) {
		element -> in = s;
	} else {
		assert(element -> in == s);
	}

	element->count++;
	s->count++;
	return element;
}

void * find (const void * set_arg, const void * element_arg) {
	const struct object * element = element_arg;

	assert(set_arg);
	assert(element);
	return element->in == set_arg ? (void *) element : 0;
}

void * drop(void * set_arg, const void * element_arg) {
	struct set * s = set_arg;
	struct object * element = find(s, element_arg);


	if(element) {
		element->count--;
		if(element->count == 0) {
			element->in = 0;
		}
		s->count--;
	}
	return element;
}


/* A new addition, the count function that lets 
 * a client know how many elements are in a set.
 */
unsigned count(const void * set_arg) {
	const struct set * s = set_arg;
	assert(s);
	return s->count;
}

/* Copied over from the original set.c
 */	
int differ(const void * a, const void * b) {
	return a != b;
}

/* Copied over from the origianl set.c
 */
int contains(const void * set_arg, const void * element_arg) { 
	return find(set_arg, element_arg) != 0;
}



