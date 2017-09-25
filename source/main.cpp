#include "../include/FR_rand.h"
#include "../include/DSC_rand.h"
#include <windows.h>

int main () {
	
	CreateDirectory("PDRX Files", NULL);

	FR_rand();
	DSC_rand();

	return 0;
}
