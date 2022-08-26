#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "archt.h"

char imgToWrite[MAX_IMGS_LENGTH][MAX_STR_LENGTH];
unsigned imgsToWriteLength;

int main()
{
    printf("Generating: %s...\n\n", IMG_ARCHIVE_NAME);

    InitImgToWrite();
    CreateImgArcht();

    // Close
    getchar();
    return 0;
}

// ================================
// Define which files are to be written
// ================================
void InitImgToWrite(void)
{
    printf("Reading Files.txt...\n\n");

    // Read all images to write from File.txt
    FILE* fp = fopen("Files.txt", "r");

    if(fp == NULL)
        printf("FATAL ERROR! Could not find 'Files.txt'\n");
    
    // Full line
    char buffer[MAX_STR_LENGTH];
    
    imgsToWriteLength = 0;
    while(fgets(buffer, MAX_STR_LENGTH, fp) != NULL)
    {
        char curLine[MAX_STR_LENGTH];   // Current line we're writing
        char* str;                      // Used to strchr
        int indx;                       // Index of the =
        int i;                          // Index for writing in new string

        // Find index for reading after the =
        str = strchr(buffer, '=');
        indx = (int)(str - buffer) + 1;

        // Init index for writing
        i = 0;
        
        // Write
        while(buffer[indx] != ';' && buffer[indx] != '\n' && buffer[indx] != EOF)
        {
            curLine[i] = buffer[indx];
            i++;
            indx++;
        }
        curLine[i] = '\0';
        printf("%s\n", curLine);
        strcpy(imgToWrite[imgsToWriteLength], curLine);
        imgsToWriteLength++;
    }

    printf("\nDone Reading Files.txt!\n\n");
}

// ================================
// Core, pack the images files to a .archt file
// ================================
void CreateImgArcht(void)
{
    printf("\nCreating archive...\n\n");

    // Pointer to destination file
    FILE* archPtr;

    // Table Of Contents
    tocElement_t toc[MAX_TOC_LENGTH];
    uint32_t tocSize = 0;

    // Archive
    byte* arch[MAX_IMG_ARCH_SIZE];                  // Raw bytes array
    unsigned long* archLength = (unsigned long *)malloc((imgsToWriteLength) * sizeof(long));;  // The sizes of the arch Image
    unsigned long currentOffset = 0;                // Current offset we're writing at
 
    // Read all the images and save the in the arch bytes array
    for(int i = 0; i < imgsToWriteLength; i++)
    {
        // Current file, buffer and length (images can have different lenght)
        FILE* filePtr;
        byte* archBuff;
        unsigned long fileLength;

        // Open file and get length
        filePtr = fopen(imgToWrite[i], "rb");
        fseek(filePtr, 0, SEEK_END);
        fileLength = ftell(filePtr);
        rewind(filePtr);


        // Allocate for the buffer and read all bytes
        archBuff = (char *)malloc(fileLength * sizeof(char));
        fread(archBuff, fileLength, 1, filePtr);

        printf("Read: %s, size %d, ID: %d...       ", archBuff, fileLength, i);

        // Save buffer to archive
        arch[i] = archBuff;             // What to write
        archLength[i] = fileLength;     // How big it is

        // Save to table of contents
        toc[i].id = i;
        toc[i].startingOffset = currentOffset;
        toc[i].size = fileLength;
        
        // Keep tocSize in bytes
        tocSize += sizeof(toc[i].id) + sizeof(toc[i].size) + sizeof(toc[i].startingOffset);

        // Move the offset to make the ToC useful
        currentOffset += fileLength;

        printf("DONE!\n");

        fclose(filePtr);
    }

    // Write the whole, final archive
    archPtr = fopen(IMG_ARCHIVE_NAME, "wb");
        
        const uint32_t ftocSize = tocSize;
        // Write TOC size
        fwrite(&ftocSize, sizeof(ftocSize), 1, archPtr);

        // Write the TOC
        for(int i = 0; i < imgsToWriteLength; i++)
        {
            fwrite(&toc[i].id, sizeof(toc[i].id), 1, archPtr);                           // Write ID
            fwrite(&toc[i].startingOffset, sizeof(toc[i].startingOffset), 1, archPtr);   // Write Offset
            fwrite(&toc[i].size, sizeof(toc[i].size), 1, archPtr);                       // Write Size
        }

        // Write the images
        for(int i = 0; i < imgsToWriteLength; i++)
            fwrite(arch[i], archLength[i], 1, archPtr);


    fclose(archPtr);

    printf("\n\nARCH BUILT as: \"%s\"\nINFO:\n", IMG_ARCHIVE_NAME);
    printf("Size of ToC in bytes: %d\n", tocSize);    
    printf("N. Elements of ToC: %d\n", tocSize / sizeof(tocElement_t));
    printf("\n\nPRESS ENTER TO QUIT\n", IMG_ARCHIVE_NAME);
    getchar();
}