#include <assert.h>
#include <stdlib.h>

/* We are now extending this to allow
 * an object to belong to multiple sets. 
 *
 * A set is now responsible for knowing
 * what objects it contains, as opposed 
 * to the objects knowing which set they 
 * are a member of. This in general is 
 * a more conventional way of implementing
 * sets, since we often want to iterate 
 * over them, rather than query random 
 * objects to see if they are pointers. 
 *
 * In technicality, this also helps us
 * separate the implementation of sets
 * and objects, since now a set doesn't 
 * have to know anything about an object - 
 * except have a pointer to it. It suffices
 * for us to declare the set to a set max. 
 * This is because delete doesn't yet know
 * how much memory was allocated for the set
 * if the set is built dynamically. So, we
 * build it statically. Lets say, max is 16. 
 *
 * This still allows us to do the exercise, 
 * which is to implement the following fns: 
 *
 * int store (const void * object, FILE *fp);
 * int storev (const void * object, va_list ap);
 *
 * int apply (const void * set,
 * 	int (* action)(void *object, va_list ap), ...);
 *
 * store: write obj info to a file. 
 * storev: to understand this, understand 
 * 	the apply fn. 
 *
 * apply: perform an action on every obj 
 * 	in the set. What is the action? 
 * 	We can pass in an fn that operates on 
 * 	objects. Clearly the fn must have obj
 * 	as a parameter, but it likely also has 
 * 	other variables that affect the action. 
 * 	these variables need to be passed in by 
 * 	the apply fn. For instance, I can say 
 * 	that the action is to print the obj 
 * 	info to a file. But, I need to pass in 
 * 	the file pointer. I pass the file ptr to
 * 	apply, which passes it to the action fn. 
 * 	This is why storev is useful - it can find
 * 	the file ptr arg in its vararg list. 
 */

/* For this set, no duplicates. Ie, adding to the 
 * set when it already has the element has no 
 * effect
 */
struct set {
	void * members[16];
	unsigned int count;
};

struct object {
	int data;
};

static const size_t set_size = sizeof(struct set);
static const size_t object_size = sizeof(struct object);

const void * set = &set_size;
const void * object = &object_size;


/* Copy over the new and delete fn's from 
 * set.c in section_8
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


/* Since calloc initializes memory to zero, we're 
 * safe, in that a set starts off as empty with count
 * == zero
 */
void *add(void * set_arg, const void *element_arg) {
	struct set * s = set_arg;
	const struct object * element = element_arg;
	int i;
	bool exists = false;

	for(i = 0; i < s->count && !exists; i++) {
		if(s->members[i] == element) {
			exists = true;
		}
	}

	if(!exists) {
		/* There is space, we can add it. */
		if(s->count < 16) {
			s->members[count] = element;
			s->count++;
		/* Can't add, return null to signify */
		} else {
			return NULL;
		}
	} 

	/* if it does exist in the set, then silently
	 * return the element, since technically it 
	 * has been "added" to the set, there was no 
	 * semantic change. 
	 */
	return element;
}


