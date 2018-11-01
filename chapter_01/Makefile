# Note: The rules need to be strealined and pattern matched. 
# The LD needs to be fixed if ld is going to be explicityly called
# For building directories like obj, do we do just obj, or obj/ ? 
# Questions questions questions. 
CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic

# Apparently the order for libraries to link in is: 
# 	crt1.o crti.o user_objects gcc_libs crtn.o
LD = ld
LDFLAGS_1 = -dynamic-linker /lib/ld-linux-x86-64.so.2 /usr/lib/crt1.o /usr/lib/crti.o 
LDFLAGS_2 = --library=c /usr/lib/crtn.o

all: dirs section10 section8 simple

dirs:
	mkdir -p obj bin

section10: obj/main_v3.o obj/set_v3.o
	gcc obj/main_v3.o obj/set_v3.o -o bin/section10

section8: obj/main.o obj/set_v2.o
	gcc obj/main.o obj/set_v2.o -o bin/section8

simple: obj/main.o obj/set.o
	$(LD) $(LDFLAGS_1) obj/main.o obj/set.o $(LDFLAGS_2) -o bin/simple

obj/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o obj/main.o

obj/main_v3.o: section_10/main.c
	$(CC) $(CFLAGS) -c section_10/main.c -o obj/main_v3.o

obj/object.o: object.c
	$(CC) $(CFLAGS) -c object.c -o obj/object.o

obj/new.o: new.c
	$(CC) $(CFLAGS) -c new.c -o obj/new.o

obj/set.o: set.c
	$(CC) $(CFLAGS) -c set.c -o obj/set.o

obj/set_v2.o: section_8/set.c
	$(CC) $(CFLAGS) -c section_8/set.c -o obj/set_v2.o

obj/set_v3.o: section_10/set.c
	$(CC) $(CFLAGS) -c section_10/set.c -o obj/set_v3.o
clean:
	rm obj/*.o
	rm bin/* 

deep_clean: 
	rm -rf obj
	rm -rf bin
