#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 *  My includes and defines.
 */
#include "coeff.h"

EQN_T*** parseSystem(FILE*, FILE*, INT_T*, INT_T*);
INT_T zmkFast(EQN_T***, INT_T, INT_T);
INT_T zmkFastDebug(EQN_T***, INT_T, INT_T);
void freeSystem(EQN_T**, INT_T);
void printSystem(EQN_T**, INT_T, INT_T);

static unsigned long long   fm_count;
static volatile bool        proceed = false;

static void done(int unused)
{
    proceed = false;
    unused = unused;
}
    
unsigned long long zmk_fm_fast(char* aname, char* cname, int seconds)
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
    INT_T* nEqn = malloc(sizeof(INT_T));
    INT_T* nVar = malloc(sizeof(INT_T));
    EQN_T*** eqns = parseSystem(afile, cfile, nEqn, nVar);

    fclose(afile);
    fclose(cfile);

    if (seconds == 0) {
        /* Just run once for validation. */
            
        INT_T res = zmkFastDebug(eqns, *nEqn, *nVar);
        free(nEqn);
        free(nVar);
        return res;
    }

    /*
     *  Tell operating system to call function DONE when an ALARM comes.
     */
    signal(SIGALRM, done);
    alarm(seconds);

    /*
     *  Now loop until the alarm comes...
     */
    proceed = true;
    while (proceed) {
        zmkFastDebug(eqns, *nEqn, *nVar);
        fm_count++;
    }

    free(nEqn);
    free(nVar);
    return fm_count;
}
