/**
 * Copyright (C) 2016 William Light <wrl@illest.net>
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

#include <wwrl/bip_buffer.h>

void
wwrl_bip_buffer_init(struct wwrl_bip_buffer *self, size_t nbytes)
{
	self->size = nbytes;

	self->b_active    = 
		self->a_start =
		self->a_end   =
		self->b_end   = 0;
}

uint8_t *
wwrl_bip_buffer_get_write_vector(struct wwrl_bip_buffer *self, size_t nbytes)
{
	if (self->b_active) {
wraparound:
		if ((self->a_start - self->b_end) < nbytes)
			return NULL;

		return &self->data[self->b_end];
	} else {
		if ((self->size - self->a_end) < nbytes)
			goto wraparound;

		return &self->data[self->a_end];
	}
}

int
wwrl_bip_buffer_write_commit(struct wwrl_bip_buffer *self, size_t nbytes)
{
	if (self->b_active) {
wraparound:
		if ((self->a_start - self->b_end) < nbytes)
			return 1;

		self->b_end += nbytes;
		return 0;
	} else {
		if ((self->size - self->a_end) < nbytes) {
			self->b_active = 1;
			self->b_end    = 0;
			goto wraparound;
		}

		self->a_end += nbytes;
		return 0;
	}
}

struct wwrl_bip_buffer_read_vector
wwrl_bip_buffer_get_read_vector(const struct wwrl_bip_buffer *self)
{
	return (struct wwrl_bip_buffer_read_vector) {
		.data       = &self->data[self->a_start],
		.nbytes     = self->a_end - self->a_start,
		.call_again = self->b_active
	};
}

int
wwrl_bip_buffer_read_commit(struct wwrl_bip_buffer *self, size_t nbytes)
{
	if ((self->a_end - self->a_start) < nbytes)
		return 1;

	self->a_start += nbytes;

	if (self->a_start == self->a_end) {
		self->a_start = 0;

		if (self->b_active) {
			self->b_active = 0;
			self->a_end    = self->b_end;
		} else
			self->a_end = 0;
	}

	return 0;
}
