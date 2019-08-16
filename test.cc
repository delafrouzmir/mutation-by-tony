#include "options.h"
#include "mutate.h"
#include "inspect.h"

#include <stdio.h>
#include <unistd.h>

#include <cstdlib>

#include <shmem.h>

static const int MASTER = 0;    // PE that gets the updates

int
main(int argc, char *argv[])
{
    shmem_init();
    const int me = shmem_my_pe();

    fill_options(argc, argv, me, MASTER);

    char *preserve = (char *) shmem_malloc(cli.seqlen);
    if (preserve == NULL) {
        if (me == MASTER) {
            fprintf(stderr, "malloc(preserve) failed\n");
        }
        shmem_global_exit(1);
    }

    char *seq = (char *) shmem_malloc(cli.seqlen);
    if (seq == NULL) {
        if (me == MASTER) {
            fprintf(stderr, "malloc(seq) failed\n");
        }
        shmem_global_exit(1);
    }

    init_seq(preserve, seq, me, MASTER);

    inspect(preserve, seq, me, MASTER);

    show_options(me, MASTER);

    // ready
    shmem_barrier_all();

    // fire off changes
    mutate(seq, me, MASTER);

    shmem_barrier_all();
    // done

    inspect(preserve, seq, me, MASTER);

    // clean up
    shmem_free(seq);
    shmem_free(preserve);

    shmem_finalize();

    return 0;
}
