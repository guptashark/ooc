#include <stdio.h>

#include "dystring.h"
#include "new.h"

int main(void) {
	void * a = new(string, "a");
	void * a2 = clone(a);
	void * b = new(string, "b");

	printf("size_of(a) == %u\n", size_of(a));

	if(differ(a, b)) puts("ok");
	if(differ(a2, a)) puts("differ?");
	if (a == a2) puts("clone?");

	delete(a);
	delete(b);
	delete(a2);

	return 0;
}
