#include "options.h"

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <shmem.h>

//
// defines bases and selection functions
//
static const char bases[] = { 'A','C','G', 'T' };
static const size_t nbases = sizeof(bases) / sizeof(bases[0]);

//
// give me a random base
//
static inline char   rand_base(void)  { return bases[rand() % nbases]; }

//
// give me a random index into the sequence
//
static inline size_t rand_index(void) { return rand() % cli.seqlen;   }

//
// populate a sequence and make a copy
//
void
init_seq(char *old, char *nova, int me, int master)
{
    if (me == master) {
        srand(getpid() + me);

        // randomly fill the base sequence
        for (size_t i = 0; i < cli.seqlen; ++i) {
            nova[i] = rand_base();
        }
        // remember for comparison
        memcpy(old, nova, cli.seqlen);
    }
}

//
// hit sequence with mutations
//
void
mutate(char *nova, int pe, int master)
{
    // pepper MASTER copy with mutations
    if (pe != master) {
        for (size_t i = 0; i < cli.exposures; ++i) {
            if ((rand() % cli.resilience) == 0) {
                const char nova_base = rand_base();
                const size_t off = rand_index();

                shmem_char_p(& (nova[off]), nova_base, master);
            }
        }
    }
}
