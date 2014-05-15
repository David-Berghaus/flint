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

#include "acb.h"

void
acb_mul(acb_t z, const acb_t x, const acb_t y, long prec)
{
#define a acb_realref(x)
#define b acb_imagref(x)
#define c acb_realref(y)
#define d acb_imagref(y)
#define e acb_realref(z)
#define f acb_imagref(z)

    if (arb_is_zero(b))
    {
        arb_mul(f, d, a, prec);
        arb_mul(e, c, a, prec);
    }
    else if (arb_is_zero(d))
    {
        arb_mul(f, b, c, prec);
        arb_mul(e, a, c, prec);
    }
    else if (arb_is_zero(a))
    {
        arb_mul(e, c, b, prec);
        arb_mul(f, d, b, prec);
        acb_mul_onei(z, z);
    }
    else if (arb_is_zero(c))
    {
        arb_mul(e, a, d, prec);
        arb_mul(f, b, d, prec);
        acb_mul_onei(z, z);
    }
    /* squaring = a^2-b^2, 2ab */
    else if (x == y)
    {
        /* aliasing */
        if (z == x)
        {
            arb_t t;
            arb_init(t);

            arb_mul(t, a, b, prec);
            arb_mul_2exp_si(t, t, 1);
            arb_mul(e, a, a, prec);
            arb_mul(f, b, b, prec);
            arb_sub(e, e, f, prec);
            arb_swap(f, t);

            arb_clear(t);
        }
        else
        {
            arb_mul(e, a, a, prec);
            arb_mul(f, b, b, prec);
            arb_sub(e, e, f, prec);
            arb_mul(f, a, b, prec);
            arb_mul_2exp_si(f, f, 1);
        }
    }
    else
    {
        /* aliasing */
        if (z == x)
        {
            arb_t t, u;

            arb_init(t);
            arb_init(u);

            arb_mul(t, a, c, prec);
            arb_mul(u, a, d, prec);

            arb_mul(e, b, d, prec);
            arb_sub(e, t, e, prec);

            arb_mul(f, b, c, prec);
            arb_add(f, u, f, prec);

            arb_clear(t);
            arb_clear(u);
        }
        else if (z == y)
        {
            arb_t t, u;

            arb_init(t);
            arb_init(u);

            arb_mul(t, c, a, prec);
            arb_mul(u, c, b, prec);

            arb_mul(e, d, b, prec);
            arb_sub(e, t, e, prec);

            arb_mul(f, d, a, prec);
            arb_add(f, u, f, prec);

            arb_clear(t);
            arb_clear(u);
        }
        else
        {
            arb_t t;
            arb_init(t);

            arb_mul(e, a, c, prec);
            arb_mul(t, b, d, prec);
            arb_sub(e, e, t, prec);

            arb_mul(f, a, d, prec);
            arb_mul(t, b, c, prec);
            arb_add(f, f, t, prec);

            arb_clear(t);
        }
    }

#undef a
#undef b
#undef c
#undef d
#undef e
#undef f
}
