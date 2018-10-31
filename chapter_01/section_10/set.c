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
 * build it statically. Lets say, max is 32. 
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


