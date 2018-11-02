#ifndef DYSTRING_H
#define DYSTRING_H

/* we can't name the file string.h since it
 * conflicts with the C string.h library. 
 * We call it dystring because it's a dynamically
 * alllocated string, and much more dynamic than 
 * C strings. However, the type itself will be a 
 * string in the global namespace
 */

extern const void * string;

#endif
