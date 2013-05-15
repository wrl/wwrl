/**
 * vector.h -- `growable` arrays in C, mimicking C++ std::vector
 * version 1.0, May 15th, 2013
 *
 * Copyright (C) 2013 William Light <wrl@illest.net>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

struct wwrl_allocator stdlib_allocator = {
    .malloc  = malloc,
    .free    = free,
    .calloc  = calloc,
    .realloc = realloc
};

VECTOR(test_vec, int);

static void populate(struct test_vec *vec)
{
	int i, use = vec->available * 2;

	for (i = 0; i < use; i++)
		VECTOR_PUSH_BACK(vec, &i);

	for (i = 50; i > 45; i--)
		VECTOR_PUSH_FRONT(vec, &i);
}

static void poppin(struct test_vec *vec)
{
	for (; vec->data[0];)
		VECTOR_POP_FRONT(vec);
	VECTOR_POP_FRONT(vec);
}

static void print_vec(struct test_vec *vec)
{
	int i;

	for (i = 0; i < vec->used; i++)
		printf("%d ", vec->data[i]);
	printf("\n");
}

int main(int argc, char **argv)
{
	struct test_vec test_vec;

    VECTOR_INIT(&test_vec, &stdlib_allocator, 10);

	populate(&test_vec);
	print_vec(&test_vec);
	poppin(&test_vec);
	print_vec(&test_vec);

    return 0;
}
