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
 *
 * Pros: 
 * 	* Easier for client to understand 
 * 	  what is going on behind the scenes, 
 * 	* Faster for C, no pointer chasing in fn's, 
 * 	assuming that the struct is small. 
 * Cons: 
 * 	* Client has access to internals. 
 * 	* changing the struct may break their code. 
 * 
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
 *
 *  Pros: 
 *  	* Client can't reasonably modify internals.
 *  Cons: 
 *  	* Pointer chasing. 
 *  	* not flexible, if representation changes
 *  	from a struct to an enum, etc. then 
 *  	the lib is in trouble since apps already
 *  	assume that it's a struct. 
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
 *
 *  Pros: 
 *  	* Super easy to upgrade, since void * 
 *  	ensures client's code can't break if 
 *  	internals are changed (within spec)
 *  	* hides everything from client. 
 *  Cons: 
 *  	* lose type safety. 
 *  	* pointer chasing. 
 */
extern const void * set;


void * add(void * set, const void * element);
void * find(const void * set, const void * element);
void * drop(void * set, const void * element);
int contains(const void * set, const void * element);

#endif
