#include <animator.h>
#include <malloc.h>
#include <mem.h>

//TODO: Remove this after
SDL_Rect *slice_array(SDL_Rect *array, int start, int end) {
    int numElements = (end - start + 1);
    size_t numBytes = sizeof(SDL_Rect) * numElements;

    SDL_Rect *slice = malloc(numBytes);
    memcpy(slice, array + start, numBytes);

    return slice;
}

animation_t newAnimation(char name[], SDL_Rect *spriteSheet, int startFrame, int endFrame,float duration, int onLoop){
    animation_t resultAnimation;

    resultAnimation.sprites = slice_array(spriteSheet, startFrame, endFrame);
    strcpy(resultAnimation.name, name);
    resultAnimation.count = 0;
    resultAnimation.frame = -1;
    resultAnimation.length = endFrame - startFrame + 1;
    resultAnimation.duration = duration;
    resultAnimation.onLoop = onLoop;

    return resultAnimation;
}

animation_t *getAnimation(struct animator *animator, char name[]){
    for (int i = 0; i < 5; ++i) {
        if(strcmp(name, animator->animations[i].name) != 0)
            continue;

        return &animator->animations[i];
    }
}

void playAnimation(animator_t *animator, char name[]){
    for (int i = 0; i < 5; ++i) {
        if(strcmp(name, animator->animations[i].name) != 0)
            continue;

        if(strcmp(animator->currentAnimation->name, animator->animations[i].name) == 0)
            break;

        animator->currentAnimation = &(animator->animations[i]);
        animator->currentAnimation->count = animator->currentAnimation->duration;
        animator->currentAnimation->frame = 0;
    }
}

void updateAnimation(SDL_Rect *srcR, animator_t *animator, float dt){
    animator->currentAnimation->count += dt;
    if(animator->currentAnimation->count >= animator->currentAnimation->duration){
        animator->currentAnimation->count = 0;
        animator->currentAnimation->frame++;

        if(animator->currentAnimation->frame >= animator->currentAnimation->length)
            animator->currentAnimation->frame = (animator->currentAnimation->onLoop) ? 0 : animator->currentAnimation->frame-1;

        *srcR = animator->currentAnimation->sprites[animator->currentAnimation->frame];
    }
}