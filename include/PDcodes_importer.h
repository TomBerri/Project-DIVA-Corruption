#include <stdint.h>

typedef struct _tagMOD_INFO
{
    char      name[9];
    uint32_t  model_id;
    uint32_t  tex_id;
} SMOD_INFO, *PSMOD_INFO;
