#ifndef CLASS_R
#define CLASS_R

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

#endif
