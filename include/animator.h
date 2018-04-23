#include <SDL.h>

typedef struct animation{
    char name[20];
    SDL_Rect *sprites;
    int frame;
    float duration;
    float count;
    float length;
    int onLoop;
} animation_t;

typedef struct animator{
    animation_t animations[5];
    animation_t *currentAnimation;
} animator_t;

animation_t animation_new(char *name, SDL_Rect *spriteSheet, int startFrame, int endFrame, float duration, int onLoop);

animation_t *animation_get(struct animator *animator, char *name);

void animation_play(animator_t *animator, char *name);

void animation_update(SDL_Rect *srcR, animator_t *animator, float dt);