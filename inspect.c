#include "options.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <shmem.h>

/*
 * mark changes
 */

static void
highlight(char c)
{
    printf("\033[1;33m" "%c" "\033[0m", c);
}

void
inspect(char *old, char *new, int me, int master)
{
    if (me == master) {
        /* show new sequence with changes */
        size_t i = 0;

        while (i < cli.seqlen) {
            if (old[i] != new[i]) {
                highlight(new[i]);
            }
            else {
                putchar(new[i]);
            }

            ++i;
            if ((i % cli.wrap) == 0) {
                putchar('\n');
            }
        }
        printf("\n\n");

        /* show changes? */
        if (cli.verbose) {
            for (i = 0; i < cli.seqlen; ++i) {
                if (old[i] != new[i]) {
                    printf("@ %4lu: %c -> %c\n",
                           (unsigned long) i, old[i], new[i]);
                }

            }
        }
    }
}
