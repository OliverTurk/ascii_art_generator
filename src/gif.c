#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gif.h"
#include "image.h"
#include "stb_image.h"

gif load_gif(char* filepath)
{
    FILE *f = fopen(filepath, "rb");
    
    if (!f)
    {
        printf("Error loading the gif %s,", stbi_failure_reason());
        exit(1); 
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buffer = malloc(size);
    fread(buffer, 1, size, f);

    int* delays = NULL;
    int width, height, num_frames, channels;
    unsigned char *pixels = stbi_load_gif_from_memory(
        buffer, (int)size,
        &delays,
        &width, &height, &num_frames,
        &channels, 0
    );
    
    return (gif){
        .width = width,
        .height = height,
        .delays = delays,
        .channels = channels,
        .num_frames = num_frames,
        .data = pixels
    };
}

gif resize_gif(gif* original, int width)
{
    int frame_size = original->height * original->width * original->channels;

    float scaling_factor = (float) width / original->width;
    int height = (original->height * scaling_factor) / 2.0f ;
    
    int resized_frame_size = width * height * original->channels;

    unsigned char* resized_buffer = malloc(width * height * original->channels * original->num_frames);

    for (int i = 0; i < original->num_frames; ++i)
    {
        unsigned char* data = original->data + i * frame_size;

        image frame = (image) {
            .width = original->width,
            .height = original->height,
            .channels = original->channels,
            .data = data
        };

        image resized_frame = resize_image(&frame, width);
        
        memcpy(resized_buffer + i * resized_frame_size, resized_frame.data, resized_frame_size);
    }

    return (gif) {
        .width = width,
        .height = height,
        .delays = original->delays,
        .channels = original->channels,
        .num_frames = original->num_frames,
        .data = resized_buffer
    };
}

void delay(int num_of_milli_seconds)
{
    clock_t start_time = clock();

    while (clock() < start_time + (num_of_milli_seconds * 1000))
        ;
}

void play_gif(gif* gif)
{
    int frame_size = gif->height * gif->width * gif->channels;
    int i = 0;

    while (1)  // infinite loop
    {
        unsigned char* data = gif->data + i * frame_size;

        image frame = (image) {
            .width = gif->width,
            .height = gif->height,
            .channels = gif->channels,
            .data = data
        };

        printf("\033[H");
        print_image(&frame);
        delay(gif->delays[i]);

        i = (i + 1) % gif->num_frames;  // loop back to first frame
    }
}
