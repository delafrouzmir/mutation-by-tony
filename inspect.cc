#include "options.h"

#include <stdio.h>

#include <cstddef>
#include <cstdlib>

#include <shmem.h>

//
// mark changes
//
static void
highlight(char c)
{
    printf("\033[1;33m" "%c" "\033[0m", c);
}

void
inspect(char *old, char *nova, int me, int master)
{
    if (me == master) {
        // show nova sequence with changes
        size_t i = 0;

        while (i < cli.seqlen) {
            if (old[i] != nova[i]) {
                highlight(nova[i]);
            }
            else {
                putchar(nova[i]);
            }

            ++i;
            if ((i % cli.wrap) == 0) {
                putchar('\n');
            }
        }
        printf("\n\n");

        // show changes?
        if (cli.verbose) {
            for (size_t i = 0; i < cli.seqlen; ++i) {
                if (old[i] != nova[i]) {
                    printf("%4lu: %c -> %c\n",
                           (unsigned long) i, old[i], nova[i]);
                }

            }
        }
    }
}
