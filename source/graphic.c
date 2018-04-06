#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <malloc.h>

SDL_Texture *loadTexture(SDL_Renderer* renderer, char path[]){
    SDL_Surface* surface = IMG_Load(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Rect *splitImage(SDL_Rect *rect, int column, int row) {

    SDL_Rect *image = (SDL_Rect *) malloc((column * row) * sizeof(SDL_Rect));
    int splitWidth = rect->w / column;
    int splitHeight = rect->h / row;
    int i = 0;
#if DEBUG
    printf("SplitWidth: %d SplitHeight: %d\n", splitWidth, splitHeight);
#endif

    for (int y = 0; y < row; ++y) {
        for (int x = 0; x < column; ++x) {
            (image + i)->x = splitWidth * x;
            (image + i)->y = splitHeight * y;
            (image + i)->w = splitWidth;
            (image + i)->h = splitHeight;
#if DEBUG
            printf("SplitImage: %d PositionX: %d PositionY: %d\n", i, (image + i)->x, (image + i)->y);
#endif
            i++;
        }
    }

    return image;
}
