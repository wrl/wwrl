CC = gcc
LD = $(CC)

CFLAGS  = -ggdb -Werror -pedantic -Wall
LDFLAGS = -rdynamic

TARGETS = test_bst test_llrb

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

test_bst: tree_bst.o tree_test.o
	$(LD) $(LDFLAGS) tree_bst.o tree_test.o -o $@

test_llrb: tree_llrb.o tree_test.o
	$(LD) $(LDFLAGS) tree_llrb.o tree_test.o -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
