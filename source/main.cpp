#include "../include/FR_rand.h"
#include "../include/DSC_rand.h"
#include <windows.h>
#include <string.h>

int main (int argc, char *argv[]) {
	
	CreateDirectory("PDRX Files", NULL); // Creates the directory for all randomised files created

	if (argc == 2 && (strcmp(argv[1], "fr") == 0 || strcmp(argv[1], "FR") == 0)) { // firstread randomiser
		FR_rand();
	} else if (argc == 3 && (strcmp(argv[1], "dsc") == 0 || strcmp(argv[1], "DSC") == 0) && ((strcmp(argv[2], "a") == 0 || strcmp(argv[1], "A") == 0) || (strcmp(argv[2], "s") == 0 || strcmp(argv[1], "S") == 0))) { // DSCs randomiser
		if (strcmp(argv[2], "a") == 0 || strcmp(argv[1], "A") == 0) {
			DSC_rand(0);
		} else{
			DSC_rand(1);
		}
	} else {
		fprintf(stderr, "Invalid Argument: Use fr/FR for firstread.bin or dsc/DSC for any/all DSC"); // Bad input argument
	}

	return 0;
}
