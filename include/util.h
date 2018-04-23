#include <SDL.h>

typedef enum bool {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef struct color {
    int r;
    int g;
    int b;
    int a;
} color_t;

SDL_Rect *slice_array(SDL_Rect *array, int start, int end);