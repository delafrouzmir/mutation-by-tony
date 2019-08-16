#include <stddef.h>             /* size_t */

typedef struct cli {
    size_t resilience;
    size_t exposures;
    size_t seqlen;
    size_t wrap;
    bool verbose;
} cli_t;

extern cli_t cli;

void fill_options(int argc, char *argv[], int me, int master);
void show_options(int me, int master);
