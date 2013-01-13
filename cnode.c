/*
 * Copyright (c) 2012-2013 Jan Klemkow <j.klemkow@wemelug.de>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

uint32_t
ipstr2int(char *str)
{
	uint32_t ip = 0;

	ip |= strtol(str, &str, 10) << 24; str++;
	ip |= strtol(str, &str, 10) << 16; str++;
	ip |= strtol(str, &str, 10) <<  8; str++;
	ip |= strtol(str, &str, 10);

	return ip;
}

uint32_t
masks2lscount(uint32_t *masks)
{
	uint32_t mask = 0;

	for (int i = 0; i < 32; ++i)
		mask |= masks[i];

	/* looking for highest bit in mask */
	for (int i = 32; i > 0; --i)
		if (mask & (1 << (i-1)))
			return i;

	return 0; /* this should not happen */
}

void
usage(void)
{
	fprintf(stderr, "cnode SRC-IP DST-IP MASK ... [MASK]\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char**argv)
{
	if (argc < 4 || argc > 35)
		usage();

	/* convert ip addresses */
	uint32_t src_ip = ipstr2int(argv[1]);
	uint32_t dst_ip = ipstr2int(argv[2]);

	uint32_t masks[32] = {0};
	int nodes = argc - 3;

	/* convert node masks */
	for (int i = 0; i < nodes; i++)
		masks[i] = strtol(argv[3+i], NULL, 10);

	/* calculate load sharing count */
	uint32_t lscount = masks2lscount(masks);

	/* calculate mask from ip address */
	uint32_t fold = src_ip ^ dst_ip;
	fold %= lscount;
	uint32_t mask = 1 << fold;

	/* match mask with given masks */
	for (int i = 0; i < nodes; ++i)
		if (masks[i] & mask)
			printf("%u\n", masks[i]);

	return EXIT_SUCCESS;
}
