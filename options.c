#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <libgen.h>

static char *progname;

cli_t cli;                      /* exposed */

static struct option opts[] = {
    { "resilience", required_argument, NULL, 'r' },
    { "exposures",  required_argument, NULL, 'e' },
    { "seqlen",     required_argument, NULL, 's' },
    { "wrap",       required_argument, NULL, 'w' },
    { "verbose",    no_argument,       NULL, 'v' },
    { NULL,         no_argument,       NULL, 0   }
};

static void
bail(void)
{
    fprintf(stderr,
            "Usage: %s [options]\n\n", progname);
    fprintf(stderr,
            "    -r N | --resilience=N    likelihood of change\n");
    fprintf(stderr,
            "    -e N | --exposures=N     number of rounds per PE\n");
    fprintf(stderr,
            "    -s N | --seqlen=N        number of bases in sequence\n");
    fprintf(stderr,
            "    -w N | --wrap=N          chop sequence lines at column N\n");
    fprintf(stderr,
            "    -v | --verbose           include change listing\n");
    fprintf(stderr,
            "\n");
}

void
fill_options(int argc, char *argv[], int me, int master)
{
    progname = basename(argv[0]);

    /* defaults */
    cli.resilience = 100;
    cli.exposures = 1000;
    cli.seqlen = 280;
    cli.wrap = 72;
    cli.verbose = 0;

    while (1) {
        const int c =
            getopt_long(argc, argv,
                        /* initial colon suppresses error messaages */
                        ":r:e:s:w:v",
                        opts, NULL);

        if (c == -1) {
            break;
        }

        switch (c) {
        case 'r':
            cli.resilience = atol(optarg);
            break;
        case 'e':
            cli.exposures = atol(optarg);
            break;
        case 's':
            cli.seqlen = atol(optarg);
            break;
        case 'w':
            cli.wrap = atol(optarg);
            break;
        case 'v':
            cli.verbose = 1;
            break;
        default:
            if (me == master) {
                bail();
                exit(1);
            }
            break;
        }
    }
}

static void
hr(void)
{
    size_t i;

    for (i = 0; i < cli.wrap; ++i) {
        printf("-");
    }
    printf("\n");
}

void
show_options(int me, int master)
{
    if (me == master) {
        hr();
        printf("Resilience       = %lu\n", (unsigned long) cli.resilience);
        printf("Exposures        = %lu\n", (unsigned long) cli.exposures);
        printf("Sequence Length  = %lu\n", (unsigned long) cli.seqlen);
        printf("Wrap Line        = %lu\n", (unsigned long) cli.wrap);
        printf("Verbose?         = %s\n",  cli.verbose ? "yes" : "no");
        hr();
        printf("\n");
    }
}
