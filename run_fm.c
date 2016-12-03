#ifndef RUN_FM_C
#define RUN_FM_C

#include "eqn.c"
#include "zmk_fm.c"
#include "zmk_fm_fast.c"
#include "zmk_fm_small.c"
#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_FAST "zmk_fast"
#define NAME_SMALL "zmk_small"

#define EQN_T short
#define SCANF_FORMAT "%d %d"
#define BUF_SIZ (50)
#define LEN     (5)
#define VARS    (4)

/**
 *  Free the equation matrix.
 */
free_eqns(EQN_T** eqns)
{
    size_t i;
    for (i = 0; i < LEN; ++i) {
        free(eqns[i]);
    }
    free(eqns);
}

/**
* Reads a file into an EQN_T array. The file is expected to have an isolated
* number on each line.
*
* @param path
*         Path to the file to read.
* @return
*         Pointer to the EQN_T array.
*/
EQN_T* parse_eqn_array(FILE* path)
{
    EQN_T *arr = (EQN_T *) malloc(sizeof(EQN_T) * LEN);
    int i;

    for (i = 0; i < LEN; ++i)
    {
        fscanf(file, "%f", &arr[i]);
    }

    return arr;
}

/**
* Reads a file into an equation block. The first file is expected to have two
* numbers on each line delimited by a singular space, the other file is
* expected to have an isolated number on each line.
*
* @param pathA
*         The file to read containing the coefficients of x and y.
* @param pathC
*         The file to read containing the constant solutions to the
*         relational equations.
* @return
*         Pointer to the equation block.
*/
EQN_T** parse_eqns_numerical(FILE* pathA, FILE* pathC)
{
    EQN_T* cs = parse_eqn_array(pathC);

    int i;
    EQN_T** pairs = (EQN_T **) malloc(sizeof(EQN_T *) * LEN);

    for (i = 0; i < LEN; ++i)
    {
        pairs[i] = (EQN_T *) malloc(sizeof(EQN_T) * 4);
        
        fscanf(fileA, SCANF_FORMAT, &pairs[i][0], &pairs[i][1]);
        pairs[i][2] = cs[i];
        pairs[i][3] = 1;
    }
    free(cs);

    return pairs;
}

/**
* Reads a file into an equation array. The first file is expected to have two
* numbers on each line delimited by a singular space, the other file is
* expected to have an isolated number on each line.
*
* @param pathA
*         Path to the file to read containing the coefficients of x and y.
* @param pathC
*         Path to the file to read containing the constant solutions to the
*         relational equations.
* @return
*         Pinter to the equation array.
*/
equation_t** parse_eqns(char *pathA, char *pathC)
{
    EQN_T** d = parse_eqns_numerical(pathA, pathC);
    equation_t** pairs = (equation_t **) malloc(sizeof(equation_t *) * LEN);
    
    int i;
    for (i = 0; i < LEN; ++i)
    {
        pairs[i] = new_equation(d[i][0], d[i][1], d[i][2], d[i][3]);
        free(d[i]);
    }
    free(d);
    
    return pairs;
}

/**
 *  Used for own debugging, ignore.
 *  <p>
 *  This program takes one argument, which is the path to a directory containing
 *  the files 'A', 'c', and 'result.' It reads the files 'A' and 'c', creates
 *  equation objects from them, and solves the set of relational equations
 *  specified by them using Fourier-Motzkin elimination. If a solution is found
 *  it prints the interval within which x can be.
 */
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        char msg[BUF_SIZ];
        sprintf(msg, "Usage: ./%s <FOLDER_PATH> [version]", argv[0]);
        error(msg);
    }
    
    char A[BUF_SIZ], c[BUF_SIZ], result[BUF_SIZ];
    
    strcpy(A, argv[1]);
    strcpy(c, argv[1]);
    strcpy(result, argv[1]);
    
    concat(A, "/A");
    concat(c, "/c");
    concat(result, "/result");

    printf("Running ");
    if (argc >= 3) {
        EQN_T** eqns = parse_eqns_numerical(A, c);

        if (!strcmp(NAME_FAST, argv[2])) {
            printf("fast.\n");
            zmk_fast_debug(eqns);
        } else if (!strcmp(NAME_SMALL, argv[2])) {
            printf("small.\n");
            zmk_small_debug(eqns);
        }
        
        free_eqn_matrix(eqns, LEN);
    } else {
        printf("regular.\n");
        equation_t** eqns = parse_eqns(A, c);
        zmk_fm(eqns);
        int i;
        
        free_equations(eqns, LEN);
    }
    
    return 0;
}

#endif
