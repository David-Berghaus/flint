/*
    Copyright (C) 2007 David Howden
    Copyright (C) 2007, 2008, 2009, 2010 William Hart
    Copyright (C) 2008 Richard Howell-Peak
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2013 Mike Hansen

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifdef T

#include "templates.h"

int
main(void)
{
    int iter;
    FLINT_TEST_INIT(state);

    flint_printf("factor_split_single....");
    fflush(stdout);

    /* Compute a random spliting polynomial then check factorization */
    for (iter = 0; iter < 100 * flint_test_multiplier(); iter++)
    {

        int len;
        TEMPLATE(T, ctx_t) ctx;
        TEMPLATE(T, poly_t) a, b, q, r;

        len = n_randint(state, 15) + 1;
        TEMPLATE(T, ctx_randtest) (ctx, state);
        TEMPLATE(T, poly_init) (a, ctx);
        TEMPLATE(T, poly_init) (b, ctx);
        TEMPLATE(T, poly_init) (q, ctx);
        TEMPLATE(T, poly_init) (r, ctx);

        TEMPLATE(T, poly_randtest_monic) (a, state, 2, ctx);

        /* random polynomial of degree len */
        while (TEMPLATE(T, poly_degree) (a, ctx) < len)
        {
            TEMPLATE(T, poly_randtest_monic) (b, state, 2, ctx);
            TEMPLATE(T, poly_mul)(a, a, b, ctx);
        }

        /* b should be a factor of a */
        TEMPLATE(T, poly_factor_split_single) (b, a, ctx);

        /* check that b divides a */
        TEMPLATE(T, poly_divrem) (q, r, a, b, ctx);
        if (!TEMPLATE(T, poly_is_zero) (r, ctx))
        {
            flint_printf("FAIL:\n");
            flint_printf
                ("Error: factor does not divide original polynomial\n");
            flint_printf("factor:\n");
            TEMPLATE(T, poly_print) (b, ctx);
            flint_printf("\n\n");
            flint_printf("polynomial:\n");
            TEMPLATE(T, poly_print) (a, ctx);
            flint_printf("\n\n");
            abort();
        }

        TEMPLATE(T, poly_clear) (a, ctx);
        TEMPLATE(T, poly_clear) (b, ctx);
        TEMPLATE(T, poly_clear) (q, ctx);
        TEMPLATE(T, poly_clear) (r, ctx);

        TEMPLATE(T, ctx_clear) (ctx);
    }
    FLINT_TEST_CLEANUP(state);
    flint_printf("PASS\n");
    return 0;
}


#endif
