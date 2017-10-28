#ifndef FR_rand_h
#define FR_rand_h
#include <stdint.h>

typedef struct _tagMOD_INFO
{
    char      name[9];
    uint32_t  model_id;
    uint32_t  tex_id;
} SMOD_INFO, *PSMOD_INFO;

typedef struct {
	int *body;
	int *hand;
	int *hair;
	int *excl;
	int bodycount;
	int handcount;
	int haircount;
	int exclcount;
} vocaloid;

int compare_opcodes_len4(unsigned char a[4], unsigned char b[4]);
void randomseArrays(int size, vocaloid *character);
char *makeModuleString (int rand_vcl, int part_num);
int checkInPDcodes (unsigned char *string, SMOD_INFO *modules, int mode);
int findModulePart(char *line, vocaloid *mik, vocaloid *rin, vocaloid *len, vocaloid *luk, vocaloid *mei, vocaloid *kai, SMOD_INFO *modules);
void FR_rand();

#endif
