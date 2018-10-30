#include <stdio.h>

#include "new.h"
#include "object.h"
#include "set.h"

/* Special note: The program doesn't
 * try to delete all the allocated memory. 
 * It simply tries to show that delete works
 * both when given a real pointer (b) and 
 * when given a null ptr. (since dropping
 * an element that isn't in a set returns
 * a null ptr). 
 */
int main(void) {
	void * s = new(set);
	void * a = add(s, new(object));
	void * b = add(s, new(object));
	void * c = new(object);

	if(contains(s, a) && contains(s, b)) {
		puts("ok");
	} 

	if(contains(s, c)) {
		puts("contains?");
	}

	if(contains(s, drop(s, a))) {
		puts("drop?");
	}

	delete(drop(s, b));
	delete(drop(s, a));

	return 0;
}
