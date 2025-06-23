#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    //open the memory card
    FILE *card = fopen(argv[1], "r");

    //buffer for a block of data
    uint8_t buffer[512];
    char filename[8];
    int counter = 0;
    FILE *img = NULL;

    //while ther's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        //crerate JPEGs from data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (counter != 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            fwrite(buffer, 1, 512, img);
            counter++;
        }
        else if (counter != 0)
        {
            fwrite(buffer, 1, 512, img);
        }
    }
    fclose(img);
    fclose(card);
}
