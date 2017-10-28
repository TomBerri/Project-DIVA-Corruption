#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/PDcodes_importer.h"

static const int KNOWN_SIZE = 950;
static const int LINE_LEN   = 22;

int main(void)
{
    FILE *PDcodes;
    SMOD_INFO modules[KNOWN_SIZE];

    PDcodes = fopen("PDcodes.txt", "rb");
    long pos = fseek(PDcodes, 0, SEEK_SET);
    
    unsigned char *buffer = (unsigned char *)malloc(LINE_LEN);
    int i = 0;
    while(fread(buffer, LINE_LEN, 1, PDcodes) == 1)
    {
        //printf("i = %d   Position = %d\n", i, pos);
        PSMOD_INFO element = &modules[i++];
        memcpy(element->name, buffer, sizeof(element->name));
        element->name[9] = 0x00;
        element->model_id = *(uint32_t *)(buffer + 12);
        element->tex_id   = *(uint32_t *)(buffer + 16);
        
        fseek(PDcodes, pos+=22, SEEK_SET);
        if(feof(PDcodes)) {break;}
    }
    fclose(PDcodes);
    free(buffer);
    
    for(i=0; i<KNOWN_SIZE; i++)
    {
        printf("SMOD_INFO[%03d]: Name = %s  Model ID = %10u  Texture ID = %10u\n",
            i,
            modules[i].name,
            modules[i].model_id,
            modules[i].tex_id);
    }
    return 1;
}

