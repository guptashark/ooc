#ifndef NEW_H
#define NEW_H

/* To create an object, we need two things: 
 * 	A) It's type
 * 	B) Arguments to initialize it. 
 */

void * new (const void * type, ...);
void * delete (void * item);

#endif
