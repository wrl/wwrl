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

#pragma once

#include <stdint.h>
#include <unistd.h>

struct wwrl_bip_buffer {
	size_t a_start, a_end, b_end;
	int b_active;

	size_t size;
	uint8_t data[];
};

struct wwrl_bip_buffer_read_vector {
	const uint8_t *data;
	size_t nbytes;

	int call_again;
};

void wwrl_bip_buffer_init(struct wwrl_bip_buffer *, size_t nbytes);

uint8_t *wwrl_bip_buffer_get_write_vector(struct wwrl_bip_buffer *,
		size_t nbytes);
int wwrl_bip_buffer_write_commit(struct wwrl_bip_buffer *, size_t nbytes);

struct wwrl_bip_buffer_read_vector wwrl_bip_buffer_get_read_vector(
		const struct wwrl_bip_buffer *);
int wwrl_bip_buffer_read_commit(struct wwrl_bip_buffer *, size_t nbytes);
