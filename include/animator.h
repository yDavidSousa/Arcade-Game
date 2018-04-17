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

animation_t newAnimation(char name[], SDL_Rect *spriteSheet, int startFrame, int endFrame,float duration, int onLoop);

animation_t *getAnimation(struct animator *animator, char name[]);

void playAnimation(animator_t *animator, char name[]);

void updateAnimation(SDL_Rect *srcR, animator_t *animator, float dt);