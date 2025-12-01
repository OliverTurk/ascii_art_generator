#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

image load_image(const char* filename)
{
    int width, height, channels;
    
    unsigned char *img = stbi_load(filename, &width, &height, &channels, 0);
    
    if (img == NULL) {
        printf("Error loading the image %s,", stbi_failure_reason());
        exit(1);
    }

    return (image) {
        .width = width,
        .height = height,
        .channels = channels,
        .data = img
    };
}

image resize_image(image* original, int width)
{
    int channels = original -> channels;

    float scaling_factor = (float) width / original->width;
    int height = (original->height * scaling_factor) / 2.0f ;
    unsigned char *resized_arr = malloc(width * height * original->channels);

    float y_ratio = (float)(original->height - 1) / (height - 1);
    float x_ratio = (float)(original->width - 1) / (width - 1);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int x_l = floor(x_ratio * j);
            int y_l = floor(y_ratio * i);
            int x_h = (int)fmin(original->width - 1, ceil(x_ratio * j));
            int y_h = (int)fmin(original->height - 1, ceil(y_ratio * i));

            float x_weight = (x_ratio * j) - x_l;
            float y_weight = (y_ratio * i) - y_l;
            
            for (int k = 0; k < channels; ++k)
            {
                float a = original->data[(y_l * original->width + x_l) * channels + k];
                float b = original->data[(y_l * original->width + x_h) * channels + k];
                float c = original->data[(y_h * original->width + x_l) * channels + k];
                float d = original->data[(y_h * original->width + x_h) * channels + k];

                float pixel = a * (1 - x_weight) * (1 - y_weight) 
                    + b * x_weight * (1 - y_weight) +
                    c * y_weight * (1 - x_weight) +
                    d * x_weight * y_weight;

                resized_arr[((i * width) + j) * channels + k] = (unsigned char)pixel;
            }
        }
    }

    return (image) {
        .width = width,
        .height = height,
        .channels = channels,
        .data = resized_arr
    };
}

void print_image(image* img)
{
    char ascii_chars[12] = { ' ', '.', '-', '=', '+', '*', 'x', '#', '$', '&', 'X', '@' };

    for (int i = 0; i < img->height; ++i)
    {
        for (int j = 0; j < img->width; ++j)
        {
            unsigned char r = img->data[((i * img->width) + j) * img->channels + 0];
            unsigned char g = img->data[((i * img->width) + j) * img->channels + 1];
            unsigned char b = img->data[((i * img->width) + j) * img->channels + 2];
            
            float brightness = 0.299f*r + 0.587f*g + 0.114f*b;

            int idx = (int)(brightness * 11.0f / 255.0f);

            char character = ascii_chars[idx];
           
            printf("%c", character);
        }
        printf("\n");
    }
}

void free_image(image* img)
{
    stbi_image_free(img->data);
}
