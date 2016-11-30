#ifndef UTIL_C
#define UTIL_C

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZ (50)

void file_close(FILE *file);
FILE* file_open(char *path);
void println(char *msg);
void swap(void *a, void *b);

/**
 *	Concatenates two strings.
 *
 *	@param str1
 *			The destination string.
 *	@param str2
 *			The string to append to str1.
 */
void concat(char* str1, char* str2)
{
	char c[BUF_SIZ];
	strcpy(c, str2);
	strcat(str1, c);
}

/**
 *	Prints a message and then exits with error code 1.
 *
 *	@param msg
 *			The message to print.
 */
void error(char* msg)
{
	println(msg);
	exit(1);
}

/**
 *	Closes a file.
 *
 *	@param file
 *			The file to close.
 */
void file_close(FILE *file)
{
	fclose(file);
}
 
/**
 *	Closes a file.
 *
 *	@param path
 *			Path to the file to open.
 *	@return
 *			A pointer to the file object.
 */
FILE* file_open(char *path)
{
	FILE *file;
	if ((file = fopen(path, "r")) == NULL)
	{
		error("Error reading file.");
	}
	return file;
}

/**
 *	Prints an array of floating point numbers in the "[a, b, c, ..., n] format."
 *
 *	@param arr
 *			The array to print.
 *	@param arr_len
 *			The number of elements in arr.
 */
void print_double_array(double* arr, size_t arr_len)
{
	int a;
	char* pre = "";
	printf("[");
	for (a = 0; a < arr_len; ++a)
	{
		printf("%s%.02lf", pre, arr[a]);
		pre = ", ";
	}
	println("]");
}

/**
 *	Prints a string and starts at the next line.
 *
 *	@param msg
 *			The message to print.
 */
void println(char *msg)
{
	printf("%s\n", msg);
}

/**
 *	Switches the direction of two pointers.
 *
 *	@param a
 *			A pointer to switch.
 *	@param b
 *			The other pointer to switch.
 */
void swap(void *a, void *b)
{
	void *t = a;
	a = b;
	b = t;
}

#endif