#include "gr.h"

int main(void)
{
    gr_ctx_t ZZn;
    int flags = GR_TEST_ALWAYS_ABLE;
    ulong n;
    int i;
    flint_rand_t state;

    flint_randinit(state);

    flint_printf("nmod32....");
    fflush(stdout);

    for (i = 0; i < 100; i++)
    {
        n = (unsigned int) n_randtest(state);
        if (n == 0)
            n = 1;
        gr_ctx_init_nmod32(ZZn, n);
        gr_test_ring(ZZn, 100, flags);
        gr_ctx_clear(ZZn);
    }

    gr_ctx_init_nmod32(ZZn, UWORD(4294967291));
    gr_test_ring(ZZn, 10000, flags);
    gr_ctx_clear(ZZn);

    gr_ctx_init_nmod32(ZZn, UWORD(4294967295));
    gr_test_ring(ZZn, 10000, flags);
    gr_ctx_clear(ZZn);

    flint_randclear(state);

    flint_cleanup();
    flint_printf(" PASS\n");
    return 0;
}
