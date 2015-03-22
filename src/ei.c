#include <err.h>
#include <stdlib.h>
#include <string.h>

#include <erl_interface.h>
#include <ei.h>

#include "complex.h"
#include "erl_comm.h"

int
main()
{
	int i, j, arity, res;
	char funp[MAXATOMLEN], buf[100], out_buf[100];
	long longp;
	struct green *res_struct;
	res = 0;

	while (read_cmd(buf) > 0) {
		i = 0;
		j = 0;
		if (ei_decode_version(buf, &i, NULL) != 0)
			errx(1, "ei_decode_version");
		if (ei_decode_tuple_header(buf, &i, &arity) != 0)
			errx(1, "ei_decode_tuple_header");
		if (ei_decode_atom(buf, &i, funp) != 0)
			errx(1, "ei_decode_atom");

		if (strncmp(funp, "foo", MAXATOMLEN) == 0) {
			if (ei_decode_long(buf, &i, &longp) != 0)
				errx(1, "ei_decode_ei_long");
			res = foo((int)longp);
			if (ei_encode_version(out_buf, &j) != 0)
				errx(1, "ei_encode_version");
			if (ei_encode_long(out_buf, &j, (long)res) != 0)
				errx(1, "ei_encode_long");
		} else if (strncmp(funp, "bar", MAXATOMLEN) == 0) {
			if (ei_decode_long(buf, &i, &longp) != 0)
				errx(1, "ei_decode_ei_long");
			res = bar((int)longp);
			if (ei_encode_version(out_buf, &j) != 0)
				errx(1, "ei_encode_version");
			if (ei_encode_long(out_buf, &j, (long)res) != 0)
				errx(1, "ei_encode_long");
		} else if (strncmp(funp, "bob", MAXATOMLEN) == 0) {
			res_struct = bob();
			if (ei_encode_version(out_buf, &j) != 0)
				errx(1, "ei_encode_version");
			if (ei_encode_tuple_header(out_buf, &j, 3))
				errx(1, "ei_encode_tuple_header");
			if (ei_encode_atom(out_buf, &j, "green"))
				errx(1, "ei_encode_atom");
			if (ei_encode_long(out_buf, &j, (long)res_struct->x) != 0)
				errx(1, "ei_encode_long");
			if (ei_encode_long(out_buf, &j, (long)res_struct->y) != 0)
				errx(1, "ei_encode_long");
		}

		write_cmd(out_buf, j);
	}

	return 0;
}
