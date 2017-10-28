#include <stdio.h>
#include <stdlib.h>
#ifndef DSC_rand_h
#define DSC_rand_h

int compare_opcodes_len12(char a[12], char b[12]);
void getRandomTarget(FILE *DSC, FILE *randDSC , int DSC_size, int mode);
void DSC_rand(int mode);

#endif
