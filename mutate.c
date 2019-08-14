#include "options.h"

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <shmem.h>

/*
 * defines basesm and selection functions
 */

static const char bases[] = { 'A','C','G', 'T' };
static const size_t nbases = sizeof(bases) / sizeof(bases[0]);

static inline char   rand_base(void)  { return bases[rand() % nbases]; }
static inline size_t rand_index(void) { return rand() % cli.seqlen;   }

void
init_seq(char *old, char *new, int me)
{
    size_t i;

    srand(getpid() + me);

    /* randomly fill the base sequence */
    for (i = 0; i < cli.seqlen; ++i) {
        new[i] = rand_base();
    }
    /* remember for comparison */
    memcpy(old, new, cli.seqlen);
}

void
mutate(char *new, int pe, int master)
{
    /* pepper MASTER copy with mutations */
    if (pe != master) {
        size_t i;

        for (i = 0; i < cli.exposures; ++i) {
            if ((rand() % cli.resilience) == 0) {
                const char new_base = rand_base();
                const size_t off = rand_index();

                shmem_char_p(& (new[off]), new_base, master);
            }
        }
    }
}
