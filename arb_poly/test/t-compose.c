/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "arb_poly.h"


int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("compose....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 3000 * arb_test_multiplier(); iter++)
    {
        slong qbits1, qbits2, rbits1, rbits2, rbits3;
        fmpq_poly_t A, B, C;
        arb_poly_t a, b, c, d;

        qbits1 = 2 + n_randint(state, 200);
        qbits2 = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);

        fmpq_poly_init(A);
        fmpq_poly_init(B);
        fmpq_poly_init(C);

        arb_poly_init(a);
        arb_poly_init(b);
        arb_poly_init(c);
        arb_poly_init(d);

        fmpq_poly_randtest(A, state, 1 + n_randint(state, 20), qbits1);
        fmpq_poly_randtest(B, state, 1 + n_randint(state, 10), qbits2);
        fmpq_poly_compose(C, A, B);

        arb_poly_randtest(c, state, 1 + n_randint(state, 20), rbits1, 4);

        arb_poly_set_fmpq_poly(a, A, rbits1);
        arb_poly_set_fmpq_poly(b, B, rbits2);
        arb_poly_compose(c, a, b, rbits3);

        if (!arb_poly_contains_fmpq_poly(c, C))
        {
            flint_printf("FAIL\n\n");
            flint_printf("bits3 = %wd\n", rbits3);

            flint_printf("A = "); fmpq_poly_print(A); flint_printf("\n\n");
            flint_printf("B = "); fmpq_poly_print(B); flint_printf("\n\n");
            flint_printf("C = "); fmpq_poly_print(C); flint_printf("\n\n");

            flint_printf("a = "); arb_poly_printd(a, 15); flint_printf("\n\n");
            flint_printf("b = "); arb_poly_printd(b, 15); flint_printf("\n\n");
            flint_printf("c = "); arb_poly_printd(c, 15); flint_printf("\n\n");

            abort();
        }

        arb_poly_set(d, a);
        arb_poly_compose(d, d, b, rbits3);
        if (!arb_poly_equal(d, c))
        {
            flint_printf("FAIL (aliasing 1)\n\n");
            abort();
        }

        arb_poly_set(d, b);
        arb_poly_compose(d, a, d, rbits3);
        if (!arb_poly_equal(d, c))
        {
            flint_printf("FAIL (aliasing 2)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);
        fmpq_poly_clear(C);

        arb_poly_clear(a);
        arb_poly_clear(b);
        arb_poly_clear(c);
        arb_poly_clear(d);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
