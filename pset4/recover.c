#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef uint8_t BYTE;
int main(int argc, char *argv[])
{

    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if(card ==  NULL)
    {
        printf("Error opening file!");
    }

    // Create a buffer for a block of data
    unsigned char buffer[512];
    //track number of images generated
    int count_images = 0;
    //create a file pointer for recovered images
    FILE *output_file =  NULL;
    // While there's still data left to read from the memory card
    char *filename = malloc(8 * sizeof(char)) ;
    while (fread(buffer, sizeof(char), 512, card) == 512)
    {
        // Create JPEGs from the data
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(count_images > 0)
            {
                fclose(output_file);
            }
            //write the jpeg names
            sprintf(filename, "%03i.jpg", count_images);
            //open output file for writing
            output_file = fopen(filename, "w");
            //count number of images found
            count_images++;

        }
        //check if the output has been used for valid input
        if(output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }

    }
    free(filename);
    fclose(card);
    fclose(output_file);

}
