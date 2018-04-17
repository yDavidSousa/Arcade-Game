#include <stdio.h>
#include <SDL.h>
#include <input.h>
#include <graphic.h>
#include <collider.h>
#include <animator.h>

const int GRAVITY = 800;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPEED = 300;
const int FPS = 60;

typedef struct entity {
    vector2_t position;
    SDL_Rect srcR, desR;
    SDL_Texture* texture;
    SDL_RendererFlip flip;
    SDL_Rect *spriteSheet;
    vector2_t velocity;
    box_collider_t boxcollider;
    animator_t animator;
} entity_t;

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

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    /* Player Load Data */
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
    player.animator.animations[0] = newAnimation("idle", player.spriteSheet, 0, 3, 0.2f, TRUE);
    player.animator.animations[1] = newAnimation("walking", player.spriteSheet, 6, 11, 0.1f, TRUE);
    player.animator.animations[2] = newAnimation("swimming", player.spriteSheet, 18, 23, 0.1f, TRUE);
    player.animator.animations[3] = newAnimation("jumping", player.spriteSheet, 12, 12, 0.1f, FALSE);
    player.animator.animations[4] = newAnimation("falling", player.spriteSheet, 13, 13, 0.1f, FALSE);
    player.animator.currentAnimation = getAnimation(&player.animator, "idle");

    //Collider
    player.boxcollider.extents.x = player.desR.w / 2;
    player.boxcollider.extents.y = player.desR.h / 2;
    player.boxcollider.center.x = player.desR.x + player.boxcollider.extents.x;
    player.boxcollider.center.y = player.desR.y + player.boxcollider.extents.y;

    /* Enemy Load Data */
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
    enemy.animator.animations[0] = newAnimation("swimming", enemy.spriteSheet, 18, 23, 0.1f, TRUE);
    enemy.animator.currentAnimation = getAnimation(&enemy.animator, "swimming");
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

        //Check inputs
        if(inputState.left == PRESSED)
            player.velocity.x += -SPEED;

        if(inputState.right == PRESSED)
            player.velocity.x += SPEED;

        if(inputState.up == PRESSED && isGround) {
            player.velocity.y = -460;
            isGround = 0;
        }

        //Update entity velocity
        player.position.x += player.velocity.x * deltaTime;
        player.velocity.y += GRAVITY * deltaTime;
        player.position.y += player.velocity.y * deltaTime;

        player.boxcollider.center.x = player.position.x + player.boxcollider.extents.x;
        player.boxcollider.center.y = player.position.y + player.boxcollider.extents.y;

        //Update box colliders
        update_boxcollider(&player.boxcollider);
        update_boxcollider(&enemy.boxcollider);

        //Collision detection
        box_collider_t minkowskiDifference = minkowski_difference(enemy.boxcollider, player.boxcollider);
        if(boxCollision(player.boxcollider, enemy.boxcollider)){
            vector2_t penetrationVector = closest_point_bounds_to_point(minkowskiDifference, VECTOR_ZERO);
            player.position = vector2_sum(player.position, penetrationVector);
            if(penetrationVector.y < 0){
                player.velocity.y = 0;
                isGround = 1;
            }

            SDL_SetTextureColorMod(player.texture, 255, 0, 0);
        }
        else
            SDL_SetTextureColorMod(player.texture, 255, 255, 255);

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
            playAnimation(&player.animator, "walking");
            player.flip = player.velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }

        if(player.velocity.y != 0)
            player.velocity.y < 0 ? playAnimation(&player.animator, "jumping") : playAnimation(&player.animator, "falling");

        if(player.velocity.x == 0 && player.velocity.y == 0)
            playAnimation(&player.animator, "idle");

        //Update entity position
        player.desR.x = (int) player.position.x;
        player.desR.y = (int) player.position.y;

        //Update animations
        updateAnimation(&player.srcR, &player.animator, deltaTime);
        updateAnimation(&enemy.srcR, &enemy.animator, deltaTime);

        //Set background color
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);

#if DEBUG
        //Box colliders render
        draw_box(player.boxcollider, renderer);
        draw_box(enemy.boxcollider, renderer);
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