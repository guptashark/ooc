#ifndef SET_H
#define SET_H

/* Several ways to provide a set data type to 
 * A) a client. Provide the whole struct: 
 *
 * struct set {
 * 	type_1 member_1;
 * 	type_2 member_2;
 * 	...
 * 	type_n member_n;
 * };
 *
 * which gives full access to internals. 
 * (ie, client code can modify internals). 
 * -----------------------------------------
 *  B) Declare a pointer to a struct. 
 *
 *  struct set;
 *
 *  Since nobody knows what a struct really is, 
 *  they can't do anything, because they don't 
 *  know internals, and don't know variables. 
 *  But, they know it's some struct. Which means
 *  that they know an implementation detail. 
 *  -----------------------------------------
 *
 *  extern const void * set;
 *
 *  No access at all, except a type descriptor. 
 *  All anyone that includes the header knows is
 *  that set is declared here for use, and is 
 *  assigned in the implementation file. And a 
 *  void pointer can be literally anything. All
 *  we're giving someone is the name: set. 
 *
 *  void pointer is not an implementation detail
 *  since it is so generic. 
 */
extern const void * set;


void * add(void * set, const void * element);
void * find(const void * set, const void * element);
void * drop(void * set, const void * element);
int contains(const void * set, const void * element);

#endif
