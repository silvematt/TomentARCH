#include "stdint.h"

// Max length the ToC can have
#define MAX_TOC_LENGTH 256

// Max Length of the final archive
#define MAX_IMG_ARCH_SIZE 256+(MAX_TOC_LENGTH)

// How many images to write
#define IMG_TO_WRITE_LENGTH 10

// Destination file
#define IMG_ARCHIVE_NAME "img.archt"

typedef	unsigned char byte;

// Elements of the Table of Content (MUST BE IN SYNCH WITH THE ENGINE) 
typedef struct tocElement_s
{
    uint32_t id;
    uint32_t startingOffset;
    uint32_t size;
} tocElement_t;

// IDs of the Images (MUST BE IN SYNCH WITH THE ENGINE) 
typedef enum imgIDs_e
{
    IMG_ID_EDEFAULT_1 = 0,
    IMG_ID_W_1,
    IMG_ID_W_1Alt,
    IMG_ID_W_2,
    IMG_ID_WD_Gate1,
    IMG_ID_WD_Gate1Alt,
    IMG_ID_F_1,
    IMG_ID_C_1,
    IMG_ID_S_Barrel1,
    IMG_ID_S_Campfire,
} imgIDs_e;

// Filenames to write
extern char* imgToWrite[IMG_TO_WRITE_LENGTH];

// ================================
// Define which files are to be written
// ================================
void InitImgToWrite(void);

// ================================
// Core, pack the images files to a .archt file
// ================================
void CreateImgArcht(void);