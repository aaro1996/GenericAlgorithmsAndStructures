.PHONY: all
CC = gcc
CFLAGS = -Wall -Werror -fpic -g
all : test_int test_map

test_int : test_int.c test_int.h DataStructures/gen_red_black_tree.c DataStructures/gen_red_black_tree.h generic_type.h
	$(CC) $(CFLAGS) -D TYPEFILE=\"../generic_type.h\" -o test_int test_int.c DataStructures/gen_red_black_tree.c

test_map : test_map.c test_map.h DataStructures/gen_red_black_tree.c DataStructures/gen_red_black_tree.h map_type.h
	$(CC) $(CFLAGS) -D TYPEFILE=\"../map_type.h\" -o test_map test_map.c DataStructures/gen_red_black_tree.c


.PHONY: clean
clean :
	rm -f *.o *~ test_int test_map testfile*

clear_tests : 
	rm testfile*
