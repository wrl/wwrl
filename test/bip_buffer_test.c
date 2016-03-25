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

#include <stdio.h>
#include <assert.h>

#include <wwrl/bip_buffer.h>

static void
print_bip(const struct wwrl_bip_buffer *bip)
{
	printf(
		" :: a_start:  %zd\n"
		" :: a_end:    %zd\n"
		" :: b_active: %d\n"
		" :: b_end:    %zd\n\n",
		bip->a_start, bip->a_end, bip->b_active, bip->b_end);
}

int
main(int argc, char **argv)
{
	struct wwrl_bip_buffer_read_vector read;
	struct wwrl_bip_buffer *bip;
	uint8_t buf[512], *write;

	bip = (void *) buf;
	wwrl_bip_buffer_init(bip, 8);

	puts(" :: start");
	print_bip(bip);

	write = wwrl_bip_buffer_get_write_vector(bip, 8);
	write[0] = 11;
	write[1] = 12;
	write[2] = 13;
	write[3] = 14;
	write[4] = 15;
	write[5] = 16;
	write[6] = 17;
	write[7] = 18;
	wwrl_bip_buffer_write_commit(bip, 8);

	puts(" :: first write");
	print_bip(bip);

	read = wwrl_bip_buffer_get_read_vector(bip);
	assert(read.nbytes == 8);
	assert(read.call_again == 0);
	assert(read.data[0] == 11);
	assert(read.data[1] == 12);
	assert(read.data[2] == 13);
	assert(read.data[3] == 14);
	assert(read.data[4] == 15);
	assert(read.data[5] == 16);
	assert(read.data[6] == 17);
	assert(read.data[7] == 18);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);

	puts(" :: first read");
	print_bip(bip);

	write = wwrl_bip_buffer_get_write_vector(bip, 7);
	write[0] = 21;
	write[1] = 22;
	write[2] = 23;
	write[3] = 24;
	write[4] = 25;
	write[5] = 26;
	write[6] = 27;
	wwrl_bip_buffer_write_commit(bip, 7);

	puts(" :: second write");
	print_bip(bip);

	read = wwrl_bip_buffer_get_read_vector(bip);
	assert(read.nbytes == 7);
	assert(read.call_again == 0);
	assert(read.data[0] == 21);
	assert(read.data[1] == 22);
	assert(read.data[2] == 23);
	assert(read.data[3] == 24);
	assert(read.data[4] == 25);
	wwrl_bip_buffer_read_commit(bip, 5);

	puts(" :: second read");
	print_bip(bip);

	write = wwrl_bip_buffer_get_write_vector(bip, 7);
	assert(write == NULL);

	write = wwrl_bip_buffer_get_write_vector(bip, 3);
	write[0] = 31;
	write[1] = 32;
	write[2] = 33;
	wwrl_bip_buffer_write_commit(bip, 3);

	puts(" :: third write");
	print_bip(bip);

	read = wwrl_bip_buffer_get_read_vector(bip);
	assert(read.nbytes == 2);
	assert(read.call_again == 1);
	assert(read.data[0] == 26);
	assert(read.data[1] == 27);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);

	puts(" :: fourth read");
	print_bip(bip);

	read = wwrl_bip_buffer_get_read_vector(bip);
	assert(read.nbytes == 3);
	assert(read.call_again == 0);
	assert(read.data[0] == 31);
	assert(read.data[1] == 32);
	assert(read.data[2] == 33);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);

	return 0;
}
