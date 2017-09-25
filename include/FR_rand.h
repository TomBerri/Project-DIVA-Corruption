#ifndef FR_rand_h
#define FR_rand_h

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

void randomseArrays(int size, vocaloid *character);
char *makeModuleString (int rand_vcl, int part_num);
char *checkInPDcodes (char * string);
char *findModulePart(char *line, vocaloid *mik, vocaloid *rin, vocaloid *len, vocaloid *luk, vocaloid *mei, vocaloid *kai);
void FR_rand();

#endif
