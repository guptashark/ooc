#include <stdio.h>

#include "dystring.h"
#include "atom.h"
#include "new.h"

int main(void) {
	void * s_a = new(string, "a");
	void * s_a2 = clone(s_a);
	void * s_b = new(string, "b");

	printf("size_of(s_a) == %u\n", size_of(s_a));

	if(differ(s_a, s_b)) puts("ok");
	if(differ(s_a2, s_a)) puts("differ?");
	if (s_a == s_a2) puts("clone?");

	delete(s_a);
	delete(s_b);
	delete(s_a2);


	/* The part for atoms. */

	void * a_x = new(atom, "x");
	void * a_x2 = clone(a_x); 
	void * a_y = new(atom, "y"); 

	printf("size of a_x) == %u\n", size_of(a_x));

	if(differ(a_x, a_y)) puts("ok");
	if(differ(a_x, a_x2)) puts("differ?"); 
	if(a_x == a_x2) puts("clone?"); 
	delete(a_x);
	delete(a_x2);
	delete(a_y);

	return 0;
}
