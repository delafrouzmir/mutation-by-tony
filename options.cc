#include "options.h"

#include <stdio.h>
#include <getopt.h>
#include <libgen.h>

#include <cstdlib>
#include <cstring>
#include <unordered_map>

static char *progname;

cli_t cli;                      // exposed

static struct option opts[] = {
    { "resilience", required_argument, NULL, 'r' },
    { "exposures",  required_argument, NULL, 'e' },
    { "seqlen",     required_argument, NULL, 's' },
    { "wrap",       required_argument, NULL, 'w' },
    { "verbose",    no_argument,       NULL, 'v' },
    { NULL,         no_argument,       NULL, 0   }
};

static std::unordered_map<char, const char *> desc = {
    { 'r', "likelihood of change" },
    { 'e', "number of rounds per PE" },
    { 's', "number of bases in sequence" },
    { 'w', "chop sequence lines at column N" },
    { 'v', "include change listing at end" }
};

static const int padding = 18;  // formatting width pad

static void
bail(void)
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: %s [options]\n", progname);
    fprintf(stderr, "\n");

    struct option *op = (struct option *) opts;

    while (op->name != NULL) {
        const bool takes = (op->has_arg == required_argument);
        const char c = (char) op->val;
        const int n = strlen(op->name);

        fprintf(stderr,
                "    -%c %s | --%s%s%*s    %s\n",
                c,
                takes ? "N" : " " ,
                op->name,
                takes ? "=N" : "  ",
                padding - n, " ",
                desc[c]);
        ++op;
    }

    fprintf(stderr, "\n");
}

void
fill_options(int argc, char *argv[], int me, int master)
{
    progname = basename(argv[0]);

    // defaults
    cli.resilience = 100;
    cli.exposures = 1000;
    cli.seqlen = 280;
    cli.wrap = 72;
    cli.verbose = false;

    while (1) {
        const int c =
            getopt_long(argc, argv,
                        // initial colon suppresses error messaages
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
            cli.verbose = true;
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
    for (size_t i = 0; i < cli.wrap; ++i) {
        putchar('-');
    }
    putchar('\n');
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
