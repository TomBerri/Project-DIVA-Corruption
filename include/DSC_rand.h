#include <stdio.h>
#include <stdlib.h>
#ifndef DSC_rand_h
#define DSC_rand_h

int compare_opcodes(char a[12], char b[12]);
void getRandomTarget(FILE *DSC, FILE *randDSC , int DSC_size);
void DSC_rand();

#endif
