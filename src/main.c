#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char **argv)
{
    int opt;
    int width = 50;

    while (( opt = getopt(argc, argv, "w:")) != -1)
    {
        switch (opt) {
            case 'w':
                printf("12345");
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

    int img_width, img_height, channels;
    unsigned char *img = stbi_load(filename, &img_width, &img_height, &channels, 0);
    
    if (img == NULL) {
        printf("Error loading the image %s,", stbi_failure_reason());
        exit(1);
    }

    float scaling_factor = (float)width / img_width;
    int height = (img_height * scaling_factor) / 2.0f ;
    unsigned char *resized_arr = malloc(width * height * channels);

    float y_ratio = (float)(img_height - 1) / (height - 1);
    float x_ratio = (float)(img_width - 1) / (width - 1);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int x_l = floor(x_ratio * j);
            int y_l = floor(y_ratio * i);
            int x_h = (int)fmin(img_width - 1, ceil(x_ratio * j));
            int y_h = (int)fmin(img_height - 1, ceil(y_ratio * i));

            float x_weight = (x_ratio * j) - x_l;
            float y_weight = (y_ratio * i) - y_l;
            
            for (int k = 0; k < channels; ++k)
            {
                float a = img[(y_l * img_width + x_l) * channels + k];
                float b = img[(y_l * img_width + x_h) * channels + k];
                float c = img[(y_h * img_width + x_l) * channels + k];
                float d = img[(y_h * img_width + x_h) * channels + k];

                float pixel = a * (1 - x_weight) * (1 - y_weight) 
                    + b * x_weight * (1 - y_weight) +
                    c * y_weight * (1 - x_weight) +
                    d * x_weight * y_weight;

                resized_arr[((i * width) + j) * channels + k] = (unsigned char)pixel;
            }
        }
    }

    char ascii_chars[12] = { ' ', '.', '-', '=', '+', '*', 'x', '#', '$', '&', 'X', '@' };

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            unsigned char r = resized_arr[((i * width) + j) * 3 + 0];
            unsigned char g = resized_arr[((i * width) + j) * 3 + 1];
            unsigned char b = resized_arr[((i * width) + j) * 3 + 2];
            
            float brightness = 0.299f*r + 0.587f*g + 0.114f*b;

            int idx = (int)(brightness * 11.0f / 255.0f);

            char character = ascii_chars[idx];
           
            printf("%c", character);
        }
        printf("\n");
    }
    
    free(resized_arr);
    stbi_image_free(img);

    return 0;
}

