#include <animator.h>
#include <util.h>
#include <mem.h>

animation_t animation_new(char *name, SDL_Rect *spriteSheet, int startFrame, int endFrame, float duration, int onLoop){
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

animation_t *animation_get(struct animator *animator, char *name){
    for (int i = 0; i < 5; ++i) {
        if(strcmp(name, animator->animations[i].name) != 0)
            continue;

        return &animator->animations[i];
    }
}

void animation_play(animator_t *animator, char *name){
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

void animation_update(SDL_Rect *srcR, animator_t *animator, float dt){
    animator->currentAnimation->count += dt;
    if(animator->currentAnimation->count >= animator->currentAnimation->duration){
        animator->currentAnimation->count = 0;
        animator->currentAnimation->frame++;

        if(animator->currentAnimation->frame >= animator->currentAnimation->length)
            animator->currentAnimation->frame = (animator->currentAnimation->onLoop) ? 0 : animator->currentAnimation->frame-1;

        *srcR = animator->currentAnimation->sprites[animator->currentAnimation->frame];
    }
}