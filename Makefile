CC = gcc
LD = $(CC)

CFLAGS  = -ggdb -std=c99 -Werror -Wall -pedantic -std=c99
LDFLAGS = -rdynamic

TARGETS = test_bst test_llrb vector_test

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

test_bst: tree_bst.o tree_test.o
	$(LD) $(LDFLAGS) tree_bst.o tree_test.o -o $@

test_llrb: tree_llrb.o tree_test.o
	$(LD) $(LDFLAGS) tree_llrb.o tree_test.o -o $@

vector_test: vector_test.o
	$(LD) $(LDFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
