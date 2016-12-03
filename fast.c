#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 *  My includes and defines.
 */
#include "run_fm.c"
#include "zmk_fm_fast.c"
#define N_FILES (6)

static unsigned long long   fm_count;
static volatile bool        proceed = false;

static void done(int unused)
{
    proceed = false;
    unused = unused;
}
    
unsigned long long name_fm(char* aname, char* cname, int seconds)
{
    FILE*       afile = fopen(aname, "r");
    FILE*       cfile = fopen(cname, "r");

    fm_count = 0;

    if (afile == NULL) {
        fprintf(stderr, "could not open file A\n");
        exit(1);
    }

    if (cfile == NULL) {
        fprintf(stderr, "could not open file c\n");
        exit(1);
    }

    /*
     *  Read A and c files.
     */
    EQN_T** eqns = parse_eqns_numerical(afile, cfile);

    fclose(afile);
    fclose(cfile);

    if (seconds == 0) {
        /* Just run once for validation. */
            
        return zmk_fast(eqns);
    }

    /* Tell operating system to call function DONE when an ALARM comes. */
    signal(SIGALRM, done);
    alarm(seconds);

    /* Now loop until the alarm comes... */
    proceed = true;
    while (proceed) {
        zmk_fast(eqns);

        fm_count++;
    }
    free_eqns(eqns);

    return fm_count;
}
