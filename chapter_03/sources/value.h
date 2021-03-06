#ifndef	VALUE_H
#define	VALUE_H

/*
 *	node types
 */

const void * Minus;
const void * Inverse;
const void * Sine;
const void * Value;
const void * Mult;
const void * Div;
const void * Remainder;
const void * Add;
const void * Sub;

/*
 *	tree management
 */

void * new (const void * type, ...);
void process (const void * tree);
void delete (void * tree);

#endif
