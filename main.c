#include <stdio.h>
#include <SDL.h>
#include <input.h>
#include <graphic.h>
#include <collider.h>
#include <malloc.h>
#include <mem.h>

const int GRAVITY = 800;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPEED = 300;
const int FPS = 60;

//TODO: Create a animator struct to save the currentAnimation and animations
typedef struct animation{
    char name[20];
    SDL_Rect *sprites;
    int frame;
    float duration;
    float count;
    float length;
    bool_t onLoop;
} animation_t;

typedef struct entity {
    vector2_t position;
    vector2_t velocity;
    SDL_Rect srcR, desR;
    box_collider_t boxcollider;
    SDL_Texture* texture;
    SDL_RendererFlip flip;
    SDL_Rect *spriteSheet;
    animation_t animations[5];
    animation_t *currentAnimation;
} entity_t;

//TODO: Create a c/h file for animation component
SDL_Rect *slice_array(SDL_Rect *array, int start, int end) {
    int numElements = (end - start + 1);
    size_t numBytes = sizeof(SDL_Rect) * numElements;

    SDL_Rect *slice = malloc(numBytes);
    memcpy(slice, array + start, numBytes);

    return slice;
}

animation_t newAnimation(char name[], SDL_Rect *spriteSheet, int startFrame, int endFrame,float duration, bool_t onLoop){
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

void playAnimation(entity_t *entity, char name[]){
    for (int i = 0; i < 5; ++i) {
        if(strcmp(name, entity->animations[i].name) != 0)
            continue;

        if(strcmp(entity->currentAnimation->name, entity->animations[i].name) == 0)
            break;

        entity->currentAnimation = &(entity->animations[i]);
        entity->currentAnimation->count = entity->currentAnimation->duration;
        entity->currentAnimation->frame = 0;
    }
}

void updateAnimation(SDL_Rect *srcR, animation_t *animation, float dt){
    animation->count += dt;
    if(animation->count >= animation->duration){
        animation->count = 0;
        animation->frame++;

        if(animation->frame >= animation->length)
            animation->frame = (animation->onLoop) ? 0 : animation->frame-1;

        *srcR = animation->sprites[animation->frame];
    }
}

int main(int argc, char* args[]){
    char gameTitle[] = "Arcade Game";
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int close_requested = 0;

    int prevTime = 0;
    int currentTime = 0;
    float deltaTime = 0;

    entity_t player, enemy;
    input_state_t inputState = {};
    const vector2_t vectorZero = {0,0};

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //Player Data
    int isGround = 0;
    player.texture = loadTexture(renderer, "content/hero.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.srcR.w, &player.srcR.h);

    //Sprite Rect
    player.desR.w = player.desR.h = 16 * 4;
    player.desR.x = player.desR.y = 0;
    player.position.x = player.position.y = 0;

    //Graphic
    player.spriteSheet = splitImage(&player.srcR, 6, 5);

    //Animation
    player.animations[0] = newAnimation("idle", player.spriteSheet, 0, 3, 0.2f, TRUE);
    player.animations[1] = newAnimation("walking", player.spriteSheet, 6, 11, 0.1f, TRUE);
    player.animations[2] = newAnimation("swimming", player.spriteSheet, 18, 23, 0.1f, TRUE);
    player.animations[3] = newAnimation("jumping", player.spriteSheet, 12, 12, 0.1f, FALSE);
    player.animations[4] = newAnimation("falling", player.spriteSheet, 13, 13, 0.1f, FALSE);
    player.currentAnimation = &(player.animations[0]);

    //Collider
    player.boxcollider.extents.x = player.desR.w / 2;
    player.boxcollider.extents.y = player.desR.h / 2;
    player.boxcollider.center.x = player.desR.x + player.boxcollider.extents.x;
    player.boxcollider.center.y = player.desR.y + player.boxcollider.extents.y;

    //Enemy Data
    enemy.texture = loadTexture(renderer, "content/hero.png");
    SDL_QueryTexture(enemy.texture, NULL, NULL, &enemy.srcR.w, &enemy.srcR.h);

    //Sprite Rect
    enemy.desR.w = enemy.desR.h = 16 * 4;
    enemy.desR.x = (SCREEN_WIDTH / 2 - enemy.desR.w /2);
    enemy.desR.y = (SCREEN_HEIGHT - enemy.desR.h);

    //Graphic
    enemy.spriteSheet = splitImage(&enemy.srcR, 6, 5);
    enemy.flip = SDL_FLIP_HORIZONTAL;

    //Animation
    enemy.animations[0] = newAnimation("swimming", enemy.spriteSheet, 18, 23, 0.1f, TRUE);
    enemy.currentAnimation = &(enemy.animations[0]);

    //Collider
    enemy.boxcollider.extents.x = enemy.desR.w / 2;
    enemy.boxcollider.extents.y = enemy.desR.h / 2;
    enemy.boxcollider.center.x = enemy.desR.x + enemy.boxcollider.extents.x;
    enemy.boxcollider.center.y = enemy.desR.y + enemy.boxcollider.extents.y;

    while(!close_requested) {
        //Time loop
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;

        updateInput(&inputState);

        if(inputState.esc == PRESSED) close_requested = 1;

        player.velocity.x = 0;

        //Collision detection
        box_collider_t minkowskiDifference = minkowski_difference(enemy.boxcollider, player.boxcollider);
        if(boxCollision(player.boxcollider, enemy.boxcollider)){
            vector2_t penetrationVector = closest_point_bounds_to_point(minkowskiDifference, vectorZero);
            player.position.x += (int)penetrationVector.x;
            player.position.y += (int)penetrationVector.y;
            SDL_SetTextureColorMod(player.texture, 255, 0, 0);
        }
        else
            SDL_SetTextureColorMod(player.texture, 255, 255, 255);

        //Check inputs
        if(inputState.left == PRESSED)
            player.velocity.x += -SPEED;

        if(inputState.right == PRESSED)
            player.velocity.x += SPEED;

        if(inputState.up == PRESSED && isGround) {
            player.velocity.y = -500;
            isGround = 0;
        }

        //Update entity velocity
        player.position.x += player.velocity.x * deltaTime;
        player.velocity.y += GRAVITY * deltaTime;
        player.position.y += player.velocity.y * deltaTime;

        //Window collision detection
        if(player.position.x <= 0)
            player.position.x = 0;

        if(player.position.y <= 0)
            player.position.y = 0;

        if(player.position.x >= SCREEN_WIDTH - player.desR.w)
            player.position.x = SCREEN_WIDTH - player.desR.w;

        if(player.position.y >= SCREEN_HEIGHT - player.desR.h) {
            player.position.y = SCREEN_HEIGHT - player.desR.h;
            player.velocity.y = 0;
            isGround = 1;
        }

        //Apply animation
        if(player.velocity.x != 0){
            playAnimation(&player, "walking");
            player.flip = player.velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }

        if(player.velocity.y != 0)
            player.velocity.y < 0 ? playAnimation(&player, "jumping") : playAnimation(&player, "falling");

        if(player.velocity.x == 0 && player.velocity.y == 0)
            playAnimation(&player, "idle");

        //Update entity position
        player.desR.x = (int) player.position.x;
        player.desR.y = (int) player.position.y;
        player.boxcollider.center.x = player.desR.x + player.boxcollider.extents.x;
        player.boxcollider.center.y = player.desR.y + player.boxcollider.extents.y;

        //Update animations
        updateAnimation(&player.srcR, player.currentAnimation, deltaTime);
        updateAnimation(&enemy.srcR, enemy.currentAnimation, deltaTime);

        //Update box colliders
        update_boxcollider(&player.boxcollider);
        update_boxcollider(&enemy.boxcollider);

        //Set background color
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);

#if DEBUG
        //Box colliders render
        draw_box(player.boxcollider, renderer);
        draw_box(enemy.boxcollider, renderer);
        draw_solid_box(minkowskiDifference, renderer);
#endif

        //Entity renders
        SDL_RenderCopyEx(renderer, enemy.texture, &enemy.srcR, &enemy.desR, 0, NULL, enemy.flip);
        SDL_RenderCopyEx(renderer, player.texture, &player.srcR, &player.desR, 0, NULL, player.flip);
        SDL_RenderPresent(renderer);

        if(currentTime < FPS)
            SDL_Delay(currentTime - (Uint32)prevTime);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyTexture(enemy.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}