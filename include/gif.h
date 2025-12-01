#ifndef MY_GIF_LIB
#define MY_GIF_LIB

typedef struct {
    int width;
    int height;
    int* delays;
    int channels;
    int num_frames;
    unsigned char* data;
} gif;

gif load_gif(char* filepath);

gif resize_gif(gif* gif, int width);

void delay(int num_of_milli_seconds);

void play_gif(gif* gif);

#endif
