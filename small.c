#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static unsigned long long   fm_count;
static volatile bool        proceed = false;

static void done(int unused)
{
    proceed = false;
    unused = unused;
}
    
unsigned long long zmk_fm_small(char* aname, char* cname, int seconds)
{
    FILE*       afile = fopen(aname, "r");
    FILE*       cfile = fopen(cname, "r");

    printf("Got here.\n");
    
    fm_count = 0;

    if (afile == NULL) {
        fprintf(stderr, "could not open file A\n");
        exit(1);
    }

    if (cfile == NULL) {
        fprintf(stderr, "could not open file c\n");
        exit(1);
    }

    fclose(afile);
    fclose(cfile);

    if (seconds == 0) {
        /* Just run once for validation. */
            
        return 0;
    }

    /* Tell operating system to call function DONE when an ALARM comes. */
    signal(SIGALRM, done);
    alarm(seconds);

    /* Now loop until the alarm comes... */
    proceed = true;
    while (proceed) {
        fm_count++;
    }

    return fm_count;
}
