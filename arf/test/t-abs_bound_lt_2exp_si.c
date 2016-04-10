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

#include "arf.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("abs_bound_lt_2exp_si....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000 * arb_test_multiplier(); iter++)
    {
        arf_t x;
        fmpz_t b;
        slong c;

        arf_init(x);
        fmpz_init(b);

        if (iter < 5)
        {
            arf_set_ui_2exp_si(x, 1, LONG_MIN + 2);
            arf_mul_2exp_si(x, x, -iter);
        }
        else if (iter < 10)
        {
            arf_set_ui_2exp_si(x, 1, WORD_MAX - 2);
            arf_mul_2exp_si(x, x, iter - 5);
        }
        else
        {
            arf_randtest_special(x, state, 2 + n_randint(state, 1000), 100);
        }

        arf_abs_bound_lt_2exp_fmpz(b, x);
        c = arf_abs_bound_lt_2exp_si(x);

        if (c == -ARF_PREC_EXACT)
        {
            if (!(arf_is_zero(x) || fmpz_cmp_si(b, -ARF_PREC_EXACT) <= 0))
            {
                flint_printf("FAIL (small/zero)\n\n");
                abort();
            }
        }
        else if (c == ARF_PREC_EXACT)
        {
            if (!(arf_is_inf(x) || arf_is_nan(x) ||
                fmpz_cmp_si(b, ARF_PREC_EXACT) >= 0))
            {
                flint_printf("FAIL (large/inf/nan)\n\n");
                abort();
            }
        }
        else
        {
            if (fmpz_cmp_si(b, c) != 0)
            {
                flint_printf("FAIL (normal)\n\n");
                flint_printf("x = "); arf_print(x); flint_printf("\n\n");
                flint_printf("b = "); fmpz_print(b); flint_printf("\n\n");
                flint_printf("c = %wd\n\n", c);
                abort();
            }
        }

        arf_clear(x);
        fmpz_clear(b);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

