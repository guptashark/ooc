#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>

extern const void * object;
int differ(const void * a, const void * b);

/* Added in for the exercises (section 10) 
 */
int store(const void * object, FILE *fp);

#endif
