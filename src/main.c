#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/image.h"
#include "../include/gif.h"
#include "stb_image.h"


int main(int argc, char **argv)
{
    int opt;
    int width = 50;

    while (( opt = getopt(argc, argv, "w:")) != -1)
    {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                break;
            case '?':
                printf("Usage: filename [-w]");
                exit(1);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Missing input file");
        exit(1);
    }

    char *filename = argv[optind++];
    
    char *file_format = filename + (strlen(filename) - 3);

    if (strcmp(file_format, "gif") == 0)
    {
        gif original = load_gif(filename);
        
        gif resized = resize_gif(&original, width);

        play_gif(&resized);
    } 
    else
    {
        image original = load_image(filename);

        image resized = resize_image(&original, width); 
        
        free_image(&original);

        print_image(&resized);
            
        free(resized.data);
    }
    
    return 0;
}

