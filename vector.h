/**
 * vector.h -- dynamic/growable arrays in C, mimicking C++ std::vector
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
#include <string.h>

struct wwrl_allocator {
	void *(*malloc)(size_t size);
	void (*free)(void *ptr);
	void *(*calloc)(size_t nmemb, size_t size);
	void *(*realloc)(void *ptr, size_t size);
};

#define VECTOR(tag, type)						\
	struct tag {							\
		struct wwrl_allocator *allocator;			\
		size_t used;						\
		size_t available;					\
		type *data;						\
	}

#define VECTOR_FRONT(vec) ((vec)->data)
#define VECTOR_BACK(vec) (&(vec)->data[(vec)->used - 1])

#define VECTOR_INIT(vec, allocatr, initial_size) do {			\
	(vec)->allocator = (allocatr);					\
	(vec)->used = 0;						\
	if (!initial_size)						\
		(vec)->available = 1;					\
	else								\
		(vec)->available = initial_size;			\
	(vec)->data = (allocatr)->calloc(sizeof(*(vec)->data),		\
			(vec)->available);				\
} while (0)

/**
 * insert
 */

#define VECTOR_INSERT_DATA(vec, where, items, count) do {		\
	if ((vec)->used + count >= (vec)->available) {			\
		(vec)->available += count;				\
		(vec)->data = (vec)->allocator->realloc((vec)->data,	\
			(vec)->available * sizeof(*(vec)->data));	\
	}								\
	memmove((vec)->data + count + where, (vec)->data + where,	\
		((vec)->used - where) * sizeof(*(vec)->data));		\
	memcpy((vec)->data + where, items,				\
			count * sizeof(*(vec)->data));			\
	(vec)->used += count;						\
} while (0)

#define VECTOR_INSERT(vec, where, item)					\
	VECTOR_INSERT_DATA(vec, where, item, 1)

/**
 * push back
 */

#define VECTOR_PUSH_BACK(vec, item) do {				\
	if ((vec)->used >= (vec)->available) {				\
		(vec)->available *= 2;					\
		(vec)->data = (vec)->allocator->realloc((vec)->data,	\
			(vec)->available * sizeof(*(vec)->data));	\
	}								\
	(vec)->data[(vec)->used] = *item;				\
	(vec)->used++;							\
} while(0)

#define VECTOR_PUSH_BACK_DATA(vec, items, count) do {			\
	if ((vec)->used + count >= (vec)->available) {			\
		(vec)->available += count;				\
		(vec)->data = (vec)->allocator->realloc((vec)->data,	\
			(vec)->available * sizeof(*(vec)->data));	\
	}								\
	memcpy(VECTOR_BACK(vec), items, count * sizeof(*(vec)->data));	\
	(vec)->used += count;						\
} while (0)

/**
 * push front
 */

#define VECTOR_PUSH_FRONT(vec, item) do {				\
	if ((vec)->used >= (vec)->available) {				\
		(vec)->available *= 2;					\
		(vec)->data = (vec)->allocator->realloc((vec)->data,	\
			(vec)->available * sizeof(*(vec)->data));	\
	}								\
	memmove((vec)->data + 1, (vec)->data,				\
			(vec)->used * sizeof(*(vec)->data));		\
	(vec)->data[0] = *item;						\
	(vec)->used++;							\
} while(0)

#define VECTOR_PUSH_FRONT_DATA(vec, items, count)			\
	VECTOR_INSERT_DATA(vec, 0, items, count)

/**
 * pop
 */

#define VECTOR_POP_BACK(vec) do {					\
	if ((vec)->used)						\
		--(vec)->used;						\
} while(0)

#define VECTOR_POP_FRONT(vec) do {					\
	if (!(vec)->used)						\
		break;							\
	(vec)->used--;							\
	memmove((vec)->data, (vec)->data + 1,				\
		(vec)->used * sizeof(*(vec)->data));			\
} while(0)

/* vim: set ts=8 sw=8: */
