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

    Copyright (C) 2016 Fredrik Johansson

******************************************************************************/

#include "arb.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("can_round_mpfr....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000000 * arb_test_multiplier(); iter++)
    {
        mpfr_t x, y1, y2;
        int r1, r2;
        arb_t t;
        slong prec;
        mpfr_rnd_t rnd;

        prec = 2 + n_randint(state, 300);

        mpfr_init2(x, 2 + n_randint(state, 300));
        mpfr_init2(y1, prec);
        mpfr_init2(y2, prec);

        arb_init(t);

        switch (n_randint(state, 5))
        {
            case 0: rnd = MPFR_RNDN; break;
            case 1: rnd = MPFR_RNDZ; break;
            case 2: rnd = MPFR_RNDU; break;
            case 3: rnd = MPFR_RNDD; break;
            default: rnd = MPFR_RNDA;
        }

        arf_randtest(arb_midref(t), state, mpfr_get_prec(x), 1 + n_randint(state, 10));
        arf_abs(arb_midref(t), arb_midref(t));
        arf_get_mpfr(x, arb_midref(t), MPFR_RNDN);
        arb_root_ui(t, t, 4, 2 + n_randint(state, 300));

        if (arb_can_round_mpfr(t, prec, rnd))
        {
            r1 = mpfr_root(y1, x, 4, rnd);
            r2 = arf_get_mpfr(y2, arb_midref(t), rnd);

            if (r1 != r2 || !mpfr_equal_p(y1, y2))
            {
                flint_printf("FAIL! %ld\n");
                flint_printf("r1 = %d, r2 = %d, prec = %wd\n", r1, r2, prec);
                flint_printf("x = "); mpfr_dump(x); flint_printf("\n");
                flint_printf("y1 = "); mpfr_dump(y1); flint_printf("\n");
                flint_printf("y2 = "); mpfr_dump(y2); flint_printf("\n");
                abort();
            }
        }

        arb_clear(t);
        mpfr_clear(x);
        mpfr_clear(y1);
        mpfr_clear(y2);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
