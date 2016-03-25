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

#include "minunit.h"

MU_TEST(test_partial_buffer)
{
	uint8_t buf[sizeof(struct wwrl_bip_buffer) + 8], *write;
	struct wwrl_bip_buffer_read_vector read;
	struct wwrl_bip_buffer *bip;

	bip = (void *) buf;
	wwrl_bip_buffer_init(bip, 8);

	write = wwrl_bip_buffer_get_write_vector(bip, 4);
	write[0] = 1;
	write[1] = 2;
	write[2] = 3;
	write[3] = 4;
	wwrl_bip_buffer_write_commit(bip, 4);

	mu_check(wwrl_bip_buffer_get_write_vector(bip, 5) == NULL);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(4, read.nbytes);
	mu_assert_int_eq(0, read.call_again);
	mu_assert_int_eq(1, read.data[0]);
	mu_assert_int_eq(2, read.data[1]);
	mu_assert_int_eq(3, read.data[2]);
	mu_assert_int_eq(4, read.data[3]);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);
}

MU_TEST(test_full_buffer)
{
	uint8_t buf[sizeof(struct wwrl_bip_buffer) + 8], *write;
	struct wwrl_bip_buffer_read_vector read;
	struct wwrl_bip_buffer *bip;

	bip = (void *) buf;
	wwrl_bip_buffer_init(bip, 8);

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

	mu_check(wwrl_bip_buffer_get_write_vector(bip, 0) != NULL);
	mu_check(wwrl_bip_buffer_get_write_vector(bip, 1) == NULL);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(read.nbytes, 8);
	mu_assert_int_eq(read.call_again, 0);
	mu_assert_int_eq(read.data[0], 11);
	mu_assert_int_eq(read.data[1], 12);
	mu_assert_int_eq(read.data[2], 13);
	mu_assert_int_eq(read.data[3], 14);
	mu_assert_int_eq(read.data[4], 15);
	mu_assert_int_eq(read.data[5], 16);
	mu_assert_int_eq(read.data[6], 17);
	mu_assert_int_eq(read.data[7], 18);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);

	write = wwrl_bip_buffer_get_write_vector(bip, 4);
	write[0] = 21;
	write[1] = 22;
	write[2] = 23;
	write[3] = 24;
	wwrl_bip_buffer_write_commit(bip, 4);

	mu_check(wwrl_bip_buffer_get_write_vector(bip, 5) == NULL);

	write = wwrl_bip_buffer_get_write_vector(bip, 4);
	write[0] = 25;
	write[1] = 26;
	write[2] = 27;
	write[3] = 28;
	wwrl_bip_buffer_write_commit(bip, 4);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(read.nbytes, 8);
	mu_assert_int_eq(read.call_again, 0);
	mu_assert_int_eq(read.data[0], 21);
	mu_assert_int_eq(read.data[1], 22);
	mu_assert_int_eq(read.data[2], 23);
	mu_assert_int_eq(read.data[3], 24);
	mu_assert_int_eq(read.data[4], 25);
	mu_assert_int_eq(read.data[5], 26);
	mu_assert_int_eq(read.data[6], 27);
	mu_assert_int_eq(read.data[7], 28);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);

	mu_check(wwrl_bip_buffer_get_write_vector(bip, 8) != NULL);
}

MU_TEST(test_overfill_buffer)
{
	uint8_t buf[sizeof(struct wwrl_bip_buffer) + 8], *write;
	struct wwrl_bip_buffer_read_vector read;
	struct wwrl_bip_buffer *bip;

	bip = (void *) buf;
	wwrl_bip_buffer_init(bip, 8);

	write = wwrl_bip_buffer_get_write_vector(bip, 9);
	mu_check(!write);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(read.nbytes, 0);
	mu_assert_int_eq(read.call_again, 0);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);
}

MU_TEST(test_wraparound)
{
	uint8_t buf[sizeof(struct wwrl_bip_buffer) + 8], *write;
	struct wwrl_bip_buffer_read_vector read;
	struct wwrl_bip_buffer *bip;

	bip = (void *) buf;
	wwrl_bip_buffer_init(bip, 8);

	write = wwrl_bip_buffer_get_write_vector(bip, 7);
	write[0] = 11;
	write[1] = 12;
	write[2] = 13;
	write[3] = 14;
	write[4] = 15;
	write[5] = 16;
	write[6] = 17;
	wwrl_bip_buffer_write_commit(bip, 7);

	mu_check(wwrl_bip_buffer_get_write_vector(bip, 1) != NULL);
	mu_check(wwrl_bip_buffer_get_write_vector(bip, 2) == NULL);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(7,  read.nbytes);
	mu_assert_int_eq(0,  read.call_again);
	mu_assert_int_eq(11, read.data[0]);
	mu_assert_int_eq(12, read.data[1]);
	mu_assert_int_eq(13, read.data[2]);
	mu_assert_int_eq(14, read.data[3]);
	mu_assert_int_eq(15, read.data[4]);
	wwrl_bip_buffer_read_commit(bip, 5);

	mu_check(wwrl_bip_buffer_get_write_vector(bip, 5) != NULL);
	mu_check(wwrl_bip_buffer_get_write_vector(bip, 6) == NULL);
	mu_check(wwrl_bip_buffer_get_write_vector(bip, 7) == NULL);
	mu_check(wwrl_bip_buffer_get_write_vector(bip, 8) == NULL);
	mu_check(wwrl_bip_buffer_get_write_vector(bip, 9) == NULL);

	write = wwrl_bip_buffer_get_write_vector(bip, 3);
	write[0] = 21;
	write[1] = 22;
	write[2] = 23;
	wwrl_bip_buffer_write_commit(bip, 3);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(2,  read.nbytes);
	mu_assert_int_eq(1,  read.call_again);
	mu_assert_int_eq(16, read.data[0]);
	mu_assert_int_eq(17, read.data[1]);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);

	read = wwrl_bip_buffer_get_read_vector(bip);
	mu_assert_int_eq(3,  read.nbytes);
	mu_assert_int_eq(0,  read.call_again);
	mu_assert_int_eq(21, read.data[0]);
	mu_assert_int_eq(22, read.data[1]);
	mu_assert_int_eq(23, read.data[2]);
	wwrl_bip_buffer_read_commit(bip, read.nbytes);
}

MU_TEST_SUITE(bip_buffer)
{
	MU_RUN_TEST(test_partial_buffer);
	MU_RUN_TEST(test_full_buffer);
	MU_RUN_TEST(test_overfill_buffer);
	MU_RUN_TEST(test_wraparound);
}

int
main(int argc, char **argv)
{
	MU_RUN_SUITE(bip_buffer);
	MU_REPORT();
	return 0;
}
