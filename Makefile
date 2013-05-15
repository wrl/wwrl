CC = gcc
LD = $(CC)

CFLAGS  = -ggdb -std=c99 -Werror -Wall -pedantic -std=c99
LDFLAGS = -rdynamic

TARGETS = vector_test

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

vector_test: vector_test.o
	$(LD) $(LDFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
