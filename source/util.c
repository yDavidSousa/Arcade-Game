#include <util.h>
#include <malloc.h>
#include <mem.h>

SDL_Rect *slice_array(SDL_Rect *array, int start, int end) {
    int numElements = (end - start + 1);
    size_t numBytes = sizeof(SDL_Rect) * numElements;

    SDL_Rect *slice = malloc(numBytes);
    memcpy(slice, array + start, numBytes);

    return slice;
}