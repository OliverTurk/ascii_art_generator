#ifndef MY_IMAGE_LIB
#define MY_IMAGE_LIB

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char* data;
} image;

image resize_image(image*, int width);

image load_image(const char* filename);

void print_image(image*);

void free_image(image*);

#endif
