#ifndef ZMK_FM_C
#define ZMK_FM_C

#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZ (50)
#define LEN 	(5)

typedef struct int_pair
{
	int *a;
	int *b;
} int_pair;

/**
 *	Allocates memory for an integer pair.
 */
 int_pair* new_int_pair(int a, int b)
 {
	int_pair* pair = (int_pair *) malloc(sizeof(int_pair));
	if (pair == NULL)
	{
		free(pair);
		error("Error allocating memory for integer pair.");
	}
	
	pair->a = (int *) malloc(sizeof(int));
	pair->b = (int *) malloc(sizeof(int));
	*(pair->a) = a;
	*(pair->b) = b;
	
	if (pair->a == NULL || pair->b == NULL)
	{
		free(pair->a);
		free(pair->b);
		free(pair);
		error("Error allocating memory for integer pair member.");
	}
	
	return pair;
 }
 
 /**
  *	Free an integer pair.
  */
 void free_int_pair(int_pair* pair)
 {
	 free(pair->a);
	 free(pair->b);
	 free(pair);
 }

 /**
  *	Reads a file entry into an array.
  */
 int* parse_ints(char* path)
 {
	FILE *file = file_open(path);
	
	int *arr = (int *) malloc(sizeof(int) * LEN);
	int i;
	
	for (i = 0; i < LEN; ++i)
	{
		fscanf(file, "%d", &arr[i]);
	}
	
	file_close(file);
	return arr;
 }
 
/**
 *	Reads a file entry into an array expecting LEN integer pairs.
 */
int_pair** parse_int_pairs(char* path)
{
	FILE *file = file_open(path);
	
	int i;
	int_pair** pairs = (int_pair **) malloc(sizeof(int_pair *) * LEN);
	
	for (i = 0; i < LEN; ++i)
	{
		int a, b;
		
		fscanf(file, "%d %d", &a, &b);
		pairs[i] = new_int_pair(a, b);
	}
	
	file_close(file);
	
	return pairs;
}

/**
 *	For testing purposes only, please ignore if present.
 */
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		char msg[BUF_SIZ];
		sprintf(msg, "Usage: ./%s <FOLDER_PATH>", argv[0]);
		error(msg);
	}
	
	char A[BUF_SIZ], c[BUF_SIZ], result[BUF_SIZ];
	
	strcpy(A, argv[1]);
	strcpy(c, argv[1]);
	strcpy(result, argv[1]);
	
	concat(A, "/A");
	concat(c, "/c");
	concat(result, "/result");
	
	int i;
	
	int_pair** pairs = parse_int_pairs(A);
	int* arr = parse_ints(c);
	int* res = parse_ints(result);
	
	for (i = 0; i < LEN; ++i)
	{
		free_int_pair(pairs[i]);
	}
	free(arr);
	free(res);
	free(pairs);
	
	return 0;
}

#endif