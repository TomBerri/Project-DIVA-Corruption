#include "../include/FR_rand.h"
#include "../include/DSC_rand.h"
#include <windows.h>
#include <string.h>

int main (int argc, char *argv[]) {
	
	CreateDirectory("PDRX Files", NULL); // Creates the directory for all randomised files created

	if (argc == 2 && (strcmp(argv[1], "fr") == 0 || strcmp(argv[1], "FR") == 0)) { // firstread randomiser
		FR_rand();
	} else if (argc == 2 && (strcmp(argv[1], "dsc") == 0 || strcmp(argv[1], "DSC") == 0)) { // DSCs randomiser
		DSC_rand();
	} else {
		fprintf(stderr, "Invalid Argument: Use fr/FR for firstread.bin or dsc/DSC for any/all DSC"); // Bad input argumentZ
	}

	return 0;
}
