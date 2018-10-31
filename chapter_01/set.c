#include <assert.h>
#include <stdlib.h>

#if ! defined MANY || MANY < 1
#define MANY 10
#endif


const void * set;
const void * object;
/* ensure nobody else can access heap 
 * except for our new and delete fn's
 */
static int heap [MANY];

/* Find the first address that points to a location that 
 * has 0. Then, mark it as used, and return it. 
 *
 * This method of using the heap lets us implement the 
 * set adt, since an objec then points to the index 
 * value of the set that contains it. Before an obj
 * gets put into a set, new needs to know that the 
 * addr is in use, so we put MEM_MAX into that spot, 
 * to know that the spot is both not in a set, and is
 * also currently allocated. 
 */
void * new (const void * type, ...) {
	int * p = NULL;
	
	/* Need to void out the type for this impl
	 * to keep strict compiler happy 
	 */
	(void)type;

	for (p = heap + 1; p < heap + MANY; ++p) {
		if(! *p) {
			break;
		}
	}
	assert(p < heap + MANY);
	*p = MANY;
	return p;
}

/* Problem: Generic pointers. We need to assign it a type. 
 * Solution: affix an _arg to the arg name in question, 
 * and then assign that argument to a typed local var. 
 *
 * So instead of: foo(void * id) { ... }
 * use: foo(void * id_arg) { int * id = id_arg;
 */
void delete (void * item_arg) {
	int * item = item_arg;

	/* Ensure that the pointer is something that can 
	 * even be deallocated (ie was allocated from heap)
	 */
	if(item) {
		assert(item > heap && item < heap + MANY);
		/* new now knows this ptr is now free */
		*item = 0;
	}
}


void * add (void * set_arg, const void * element_arg) {
	int * set = set_arg;
	const int * element = element_arg;

	/* ensure set is a valid heap ptr */
	assert(set > heap && set < heap + MANY);
	/* ensure set is not an element of a heap */
	assert(*set == MANY);
	/* ensure element is a valid heap ptr */
	assert(element > heap && element < heap + MANY);

	/* if free obj, assign it to index of set that
	 * now contains it. Need to cast away const 
	 * in order to modify it. Otherwise, ensure
	 * that the given element is a member of the set.
	 * (can't add an element to two sets) 
	 */
	if (*element == MANY) {
		*(int *) element = set - heap;
	} else {
		assert(*element == set - heap);
	}

	return (void *)element;
}


/* Fairly simple. ensure set and element are both valid heap
 * ptrs, that set is a set, that element is not a free ptr, 
 * then check that element has the right index.
 */
void * find (const void * set_arg, const void * element_arg) {
	const int * set = set_arg;
	const int * element = element_arg;

	assert(set > heap && set < heap + MANY);
	assert(element > heap && element < heap + MANY);
	assert(*set == MANY);
	assert(*element);

	return *element == set - heap ? (void *)element : 0;
}

/* Very simple function, basically a formatter for the find fn. 
 * Since find returns a heap allocated ptr, that ptr can't be 
 * null, which means that the set contains it.*/
int contains(const void * set_arg, const void * element_arg) { 
	return find(set_arg, element_arg) != 0;
}

/* Assuming element is in the set, set it to MANY, so that
 * it can be added to a different set. If it's not in the set, 
 * then the element var is 0, and 0 is returned. 
 */
void * drop (void * set_arg, const void * element_arg) {
	int * element = find (set_arg, element_arg);
	if(element) {
		*element = MANY;
	}

	return element;
}

int differ(const void * a, const void * b) {
	return a != b;
}


