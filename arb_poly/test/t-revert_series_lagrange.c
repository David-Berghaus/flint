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

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "arb_poly.h"


int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("revert_series_lagrange....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000 * arb_test_multiplier(); iter++)
    {
        slong qbits1, rbits1, rbits2, n;
        fmpq_poly_t A, B;
        arb_poly_t a, b, c;

        qbits1 = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        n = 2 + n_randint(state, 25);

        fmpq_poly_init(A);
        fmpq_poly_init(B);

        arb_poly_init(a);
        arb_poly_init(b);
        arb_poly_init(c);

        do {
            fmpq_poly_randtest(A, state, 1 + n_randint(state, 25), qbits1);
            fmpq_poly_set_coeff_ui(A, 0, 0);
        } while (A->length < 2 || fmpz_is_zero(A->coeffs + 1));

        fmpq_poly_revert_series(B, A, n);

        arb_poly_set_fmpq_poly(a, A, rbits1);
        arb_poly_revert_series_lagrange(b, a, n, rbits2);

        if (!arb_poly_contains_fmpq_poly(b, B))
        {
            flint_printf("FAIL\n\n");
            flint_printf("n = %wd, bits2 = %wd\n", n, rbits2);

            flint_printf("A = "); fmpq_poly_print(A); flint_printf("\n\n");
            flint_printf("B = "); fmpq_poly_print(B); flint_printf("\n\n");

            flint_printf("a = "); arb_poly_printd(a, 15); flint_printf("\n\n");
            flint_printf("b = "); arb_poly_printd(b, 15); flint_printf("\n\n");

            abort();
        }

        arb_poly_set(c, a);
        arb_poly_revert_series_lagrange(c, c, n, rbits2);
        if (!arb_poly_equal(c, b))
        {
            flint_printf("FAIL (aliasing)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);

        arb_poly_clear(a);
        arb_poly_clear(b);
        arb_poly_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
