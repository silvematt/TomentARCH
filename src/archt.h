#include "stdint.h"

// Max length the ToC can have
#define MAX_TOC_LENGTH 256

// Max Length of the final archive
#define MAX_IMG_ARCH_SIZE 256+(MAX_TOC_LENGTH)

// Max length a string can have in Files.txt
#define MAX_STR_LENGTH 256

// Max number of imges Files.txt
#define MAX_IMGS_LENGTH 256

// Destination file
#define IMG_ARCHIVE_NAME "img.archt"


// Elements of the Table of Content (MUST BE IN SYNCH WITH THE ENGINE) 
typedef struct tocElement_s
{
    uint32_t id;
    uint32_t startingOffset;
    uint32_t size;
} tocElement_t;

// Filenames to write
extern char imgToWrite[MAX_IMGS_LENGTH][MAX_STR_LENGTH];
extern unsigned imgsToWriteLength;

typedef	unsigned char byte;

// ================================
// Define which files are to be written
// ================================
void InitImgToWrite(void);

// ================================
// Core, pack the images files to a .archt file
// ================================
void CreateImgArcht(void);