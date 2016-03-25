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

#include <wwrl/vector.h>

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(*a))

struct wwrl_allocator stdlib_allocator = {
    .malloc  = malloc,
    .free    = free,
    .calloc  = calloc,
    .realloc = realloc
};

VECTOR(test_vec, int);

/**
 * test harness stuff
 */

struct vector_state {
	unsigned size;
	int *data;
};

struct vector_state expected[] = {
	/* initial state  */ {0, NULL},
	/* push back      */ {10, (int []) {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	/* push front     */ {15, (int []) {46, 47, 48, 49, 50, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	/* insert         */ {16, (int []) {46, 47, 48, 49, 50, 0, 1, 42, 2, 3, 4, 5, 6, 7, 8, 9}},
	/* pop back       */ {11, (int []) {46, 47, 48, 49, 50, 0, 1, 42, 2, 3, 4}},
	/* erase          */ {10, (int []) {46, 47, 48, 49, 0, 1, 42, 2, 3, 4}},
	/* pop front      */ {5,  (int []) {1, 42, 2, 3, 4}},
	/* clear          */ {0, NULL},
	/* push back data */ {7,  (int []) {2, 4, 6, 8, 10, 12, 42}},
	/* push back data */ {10, (int []) {42, 42, 42, 2, 4, 6, 8, 10, 12, 42}},
	/* insert data    */ {15, (int []) {42, 42, 42, 2, 4, 0, 0, 0, 0, 0, 6, 8, 10, 12, 42}},
	/* erase range    */ {12, (int []) {42, 42, 0, 0, 0, 0, 0, 6, 8, 10, 12, 42}},

	{0, NULL}
};

static void print_array(int *a, size_t count)
{
	unsigned i;

	for (i = 0; i < count; i++)
		printf("%d ", a[i]);
	printf("\n");
}

static void check_vector_step(struct test_vec *vec, int step)
{
	struct vector_state *s = &expected[step];

	if (vec->size != s->size) {
		printf("    FAIL.\n\n");
		printf("  vec->size is %ld, should have been %d\n\n",
				vec->size, s->size);
		abort();
	}

	if (memcmp(vec->data, s->data, vec->size * sizeof(*vec->data))) {
		printf("    FAIL.\n\n");
		printf("  vector contents incorrect. expected:\n"
		       "      ");
		print_array(s->data, s->size);
		printf("  got:\n"
		       "      ");
		print_array(vec->data, vec->size);
		printf("\n");
		abort();
	}

	puts("    ok.");
}

/**
 * tests
 */

#define TEST_CASE(for_what) \
	static void test_##for_what(struct test_vec *vec)

TEST_CASE(push_back)
{
	int i, use = vec->capacity * 2;

	for (i = 0; i < use; i++)
		VECTOR_PUSH_BACK(vec, &i);
}

TEST_CASE(push_front)
{
	int i;

	for (i = 50; i > 45; i--)
		VECTOR_PUSH_FRONT(vec, &i);
}

TEST_CASE(insert)
{
	int i = 42;
	VECTOR_INSERT(vec, 7, &i);
}

TEST_CASE(pop_back)
{
	for (; *VECTOR_BACK(vec) > 4;)
		VECTOR_POP_BACK(vec);
}

TEST_CASE(erase)
{
	VECTOR_ERASE(vec, 4);
}

TEST_CASE(pop_front)
{
	for (; vec->data[0];)
		VECTOR_POP_FRONT(vec);
	VECTOR_POP_FRONT(vec);
}

TEST_CASE(clear)
{
	VECTOR_CLEAR(vec);
	VECTOR_SHRINK_TO_FIT(vec);

	assert(vec->capacity == 1);
}

TEST_CASE(push_back_data) {
	int data[] = {2, 4, 6, 8, 10, 12, 42};
	VECTOR_PUSH_BACK_DATA(vec, data, ARRAY_LENGTH(data));
}

TEST_CASE(push_front_data) {
	int data[] = {42, 42, 42};
	VECTOR_PUSH_FRONT_DATA(vec, data, ARRAY_LENGTH(data));
}

TEST_CASE(insert_data) {
	int data[] = {0, 0, 0, 0, 0};
	VECTOR_INSERT_DATA(vec, 5, data, ARRAY_LENGTH(data));
}

TEST_CASE(erase_range) {
	VECTOR_ERASE_RANGE(vec, 2, 5);
}

/**
 * and let's run them
 */

int main(int argc, char **argv)
{
	struct test_vec test_vec = {NULL, 0, 0, NULL};
	int step = 0;

#define CHECK check_vector_step(&test_vec, step++)
#define RUN_TEST(which) do {  \
	puts("  testing " #which); \
	test_##which(&test_vec);  \
	CHECK;                    \
} while (0)

	printf("\n"
			"  initializing vector\n");
    VECTOR_INIT(&test_vec, &stdlib_allocator, 5);
	CHECK;

	RUN_TEST(push_back);
	RUN_TEST(push_front);
	RUN_TEST(insert);
	RUN_TEST(pop_back);
	RUN_TEST(erase);
	RUN_TEST(pop_front);
	RUN_TEST(clear);

	puts("");
	RUN_TEST(push_back_data);
	RUN_TEST(push_front_data);
	RUN_TEST(insert_data);
	RUN_TEST(erase_range);

	VECTOR_FREE(&test_vec);

	printf("\n");

    return 0;
}
