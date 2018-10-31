CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic

# Apparently the order for libraries to link in is: 
# 	crt1.o crti.o user_objects gcc_libs crtn.o
LD = ld
LDFLAGS_1 = -dynamic-linker /lib/ld-linux-x86-64.so.2 /usr/lib/crt1.o /usr/lib/crti.o 
LDFLAGS_2 = --library=c /usr/lib/crtn.o

section8: main.o section_8/set.o
	gcc main.o section_8/set.o 

all: main.o set.o
	$(LD) $(LDFLAGS_1) main.o set.o $(LDFLAGS_2)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c


object.o: object.c
	$(CC) $(CFLAGS) -c object.c

new.o: new.c
	$(CC) $(CFLAGS) -c new.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

section_8/set.o: section_8/set.c
	$(CC) $(CFLAGS) -c section_8/set.c -o section_8/set.o

clean:
	rm -f *.o
	rm -f a.out
