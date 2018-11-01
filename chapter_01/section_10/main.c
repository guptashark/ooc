#include <stdio.h>

#include "../new.h"
#include "../object.h"
#include "../set.h"

/* Special note: The program doesn't
 * try to delete all the allocated memory. 
 * It simply tries to show that delete works
 * both when given a real pointer (b) and 
 * when given a null ptr. (since dropping
 * an element that isn't in a set returns
 * a null ptr). 
 */


/* Funnily enough, we don't really need 
 * the set and object type descriptors, 
 * since the internal representation 
 * handles them as the same, differentiating
 * only by wether a ptr is used as a set or 
 * obj. But it makes things more easy 
 * to visualize here in terms of what is 
 * happening, so we use the descriptors. 
 */ 

/* This is for v3, where the functions
 * store, storev and apply need to be used. 
 * Hence, we use another main file instead 
 * of working with the old one that doesn't
 * have the implementations for store, 
 * storev, etc. 
 */

int main(void) {
	void * s = new(set);
	void * a = add(s, new(object));
	void * b = add(s, new(object));
	void * c = new(object);

	store(a, stdout);
	store(b, stdout);

	if(contains(s, a) && contains(s, b)) {
		puts("ok");
	} 

	if(contains(s, c)) {
		puts("contains?");
	}

	if(differ(a, add(s, a))) {
		puts("differ?");
	}

	
	if(contains(s, drop(s, a))) {
		puts("drop?");
	}

	delete(drop(s, b));
	delete(drop(s, a));

	return 0;
}
