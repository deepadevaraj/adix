#include <string.h>
#include <stdio.h>
#include <syscall.h>

int printf(const char *format, ...){
	char buffer[BUF_SIZE]; //TODO: Malloc
    __builtin_va_list ap;
	int num_char;
	/* prepare valist from arguments */
    __builtin_va_start(ap, format);
	num_char = sprintf_va(buffer, format,&ap);	
	/* Print onto the console */
	uprintf(buffer);
    return num_char;

}
