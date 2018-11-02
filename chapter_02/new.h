#ifndef NEW_H
#define NEW_H


void * new(const void * class_arg, ...);
void * clone(const void * item);

void delete(void * item);

int differ(const void * self, const void * b);

size_t size_of(const void *self);

#endif
