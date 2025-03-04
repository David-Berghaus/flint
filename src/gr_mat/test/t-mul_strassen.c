#include "ulong_extras.h"
#include "gr_mat.h"

int main(void)
{
    slong iter;
    flint_rand_t state;

    flint_printf("mul_strassen...");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000; iter++)
    {
        gr_ctx_t ctx;
        gr_mat_t A, B, C, D;
        slong a, b, c;
        int status = GR_SUCCESS;

        if (n_randint(state, 2))
            gr_ctx_init_fmpz(ctx);
        else
            gr_ctx_init_nmod(ctx, n_randtest_not_zero(state));

        a = n_randint(state, 8);
        b = n_randint(state, 8);
        c = n_randint(state, 8);

        gr_mat_init(A, a, b, ctx);
        gr_mat_init(B, b, c, ctx);
        gr_mat_init(C, a, c, ctx);
        gr_mat_init(D, a, c, ctx);

        status |= gr_mat_randtest(A, state, ctx);
        status |= gr_mat_randtest(B, state, ctx);
        status |= gr_mat_randtest(C, state, ctx);
        status |= gr_mat_randtest(D, state, ctx);

        if (b == c && n_randint(state, 2))
        {
            status |= gr_mat_set(C, A, ctx);
            status |= gr_mat_mul_strassen(C, C, B, ctx);
        }
        else if (a == b && n_randint(state, 2))
        {
            status |= gr_mat_set(C, B, ctx);
            status |= gr_mat_mul_strassen(C, A, C, ctx);
        }
        else
        {
            status |= gr_mat_mul_strassen(C, A, B, ctx);
        }

        status |= gr_mat_mul_classical(D, A, B, ctx);

        if (status != GR_SUCCESS && gr_mat_equal(C, D, ctx) == T_FALSE)
        {
            flint_printf("FAIL:\n");
            gr_ctx_println(ctx);
            flint_printf("A:\n"); gr_mat_print(A, ctx); flint_printf("\n\n");
            flint_printf("B:\n"); gr_mat_print(B, ctx); flint_printf("\n\n");
            flint_printf("C:\n"); gr_mat_print(C, ctx); flint_printf("\n\n");
            flint_printf("D:\n"); gr_mat_print(D, ctx); flint_printf("\n\n");
            flint_abort();
        }

        gr_mat_clear(A, ctx);
        gr_mat_clear(B, ctx);
        gr_mat_clear(C, ctx);
        gr_mat_clear(D, ctx);

        gr_ctx_clear(ctx);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return 0;
}
