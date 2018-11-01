#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
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
	unsigned i;
	bool exists = false;


	for(i = 0; i < s->count && !exists; i++) {
		if(s->members[i] == element) {
			exists = true;
		}
	}

	if(!exists) {
		/* There is space, we can add it. */
		if(s->count < 16) {
			s->members[s->count] = (void *)element;
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
	return (void *)element;
}

/* Probably should use this fn in the add fn... */
void * find(const void * set_arg, const void * element_arg) {
	const struct set * s = set_arg;
	const struct object *element = element_arg;
	unsigned i;
	bool exists = false;
	for(i = 0; i < s->count; i++) {
		if(s->members[i] == element) {
			exists = true;
		}
	}

	if(exists) {
		return (void *)element;
	} else {
		return NULL;
	}
}

void * drop(void * set_arg, const void * element_arg) {
	struct set * s = set_arg;
	struct object * element = find(s, element_arg);
	unsigned i = 0;
	/* unsigned j; */
	bool found = false;

	/* if element is NULL, then we can't drop it, 
	 * it's not in the set. return NULL since 
	 * we couldn't drop it
	 */

	if(element == NULL) {
		return NULL;
	}
/*	
	for(i = 0; i < s->count && !found; i++) {
		if(s->members[i] == element) {
			found = true;
			j = i;
		}
	}
*/
	while(i < s->count && !found) {
		if(s->members[i] == element) {
			found = true;
		} else {
			i++;
		}
	}

	s->count--;
	s->members[i] = s->members[s->count];
	s->members[s->count] = NULL;
	return element;
}

/* copied from set_v2 (in section_8
 */
unsigned count(const void * set_arg) {
	const struct set * s = set_arg;
	return s->count;
}

int differ(const void * a, const void *b) {
	return a != b;
}

int contains(const void * set_arg, const void *element_arg) {
	return find(set_arg, element_arg) != NULL;
}


/* The actual assignment part - this will help us debug
 * the current buggy code, assuming this works./
 */
int store(const void * object_arg, FILE *fp) {
	const struct object *o = object_arg;
	return fprintf(fp, "%p\n", (void *)o);
}

int apply_store(const void * set_arg, FILE *fp) {
	const struct set * s = set_arg;
	unsigned i = 0;
	printf("%u\n", s->count);


	while(i < s->count) {
		store(s->members[i], fp);
		i++;
	}
	
	return 0;
}	



