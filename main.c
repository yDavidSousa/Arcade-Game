#include <stdio.h>
#include <SDL.h>
#include <string.h>

#include <input.h>
#include <graphic.h>
#include <collider.h>
#include <animator.h>

const int GRAVITY = 800;
const int SPEED = 300;

const int FPS = 60;

typedef enum player_state {
    IDLE,
    WALKING,
    JUMPING,
    FALLING
} player_state_t;

typedef struct window {
    char title[256];
    char icon[256];
    int width, height;
    int x, y;
    Uint32 flags;
} window_t;

typedef struct config {
    char author[256];
    char version[256];
    window_t window;
} config_t;

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

void init_default_config(config_t *config){
    strcpy(config->author, "unknown");
    strcpy(config->version, "0.0");

    strcpy(config->window.title, "None Title");
    config->window.width = 640;
    config->window.height = 480;
    config->window.x = SDL_WINDOWPOS_CENTERED;
    config->window.y = SDL_WINDOWPOS_CENTERED;
    config->window.flags = 0;
}

void read_config_file(char *path, config_t *config){
    FILE *file = fopen(path, "r");

    if(!file){
        printf("Couldn't open file\n");
        return;
    }

    char buffer[256] = {};
    while(!feof(file)){
        fscanf(file, "%s", buffer);

        if(strcmp(buffer, "author") == 0){
            fscanf(file, " %[^\n]s", config->author);
            continue;
        }
        if(strcmp(buffer, "version") == 0){
            fscanf(file, "%s", config->version);
            continue;
        }
        if(strcmp(buffer, "title") == 0){
            fscanf(file, " %[^\n]s", config->window.title);
            continue;
        }
        if(strcmp(buffer, "icon") == 0){
            fscanf(file, "%s", config->window.icon);
            continue;
        }
        if(strcmp(buffer, "fullscreen") == 0){
            fscanf(file, "%s", buffer);

            if(strcmp(buffer, "true") == 0){
                config->window.flags |= SDL_WINDOW_FULLSCREEN;
            }
            continue;
        }
        if(strcmp(buffer, "resizable") == 0){
            fscanf(file, "%s", buffer);

            if(strcmp(buffer, "true") == 0){
                config->window.flags |= SDL_WINDOW_RESIZABLE;
            }
            continue;
        }
        if(strcmp(buffer, "borderless") == 0){
            fscanf(file, "%s", buffer);

            if(strcmp(buffer, "true") == 0){
                config->window.flags |= SDL_WINDOW_BORDERLESS;
            }
            continue;
        }
        if(strcmp(buffer, "width") == 0){
            fscanf(file, "%d", &config->window.width);
            continue;
        }
        if(strcmp(buffer, "height") == 0){
            fscanf(file, "%d", &config->window.height);
            continue;
        }
        if(strcmp(buffer, "x") == 0){
            fscanf(file, "%d", &config->window.x);
            continue;
        }
        if(strcmp(buffer, "y") == 0){
            fscanf(file, "%d", &config->window.y);
            continue;
        }
    }

    fclose(file);
}

void pre_config_window(SDL_Window *window,SDL_Renderer *renderer, config_t *config){
    init_default_config(config);
    read_config_file("../config.txt", config);
}

int main(int argc, char* args[]){
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    config_t *config;
    int quit = 0;

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    pre_config_window(window, renderer, config);
    window = SDL_CreateWindow(
            config->window.title,
            config->window.x,
            config->window.y,
            config->window.width,
            config->window.height,
            config->window.flags
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int prevTime = 0;
    int currentTime = 0;
    float deltaTime = 0;

    entity_t player, enemy;
    input_state_t inputState = {};

    /* Player Load Data */
    player_state_t playerState = IDLE;
    int isGround = 0;
    player.texture = load_texture(renderer, "content/hero.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.srcR.w, &player.srcR.h);
    player.desR.w = player.desR.h = 16 * 4;
    player.desR.x = player.desR.y = 0;
    player.position.x = player.position.y = 0;
    player.spriteSheet = split_image(&player.srcR, 6, 5);
    player.animator.animations[0] = animation_new("idle", player.spriteSheet, 0, 3, 0.2f, TRUE);
    player.animator.animations[1] = animation_new("walking", player.spriteSheet, 6, 11, 0.1f, TRUE);
    player.animator.animations[2] = animation_new("swimming", player.spriteSheet, 18, 23, 0.1f, TRUE);
    player.animator.animations[3] = animation_new("jumping", player.spriteSheet, 12, 12, 0.1f, FALSE);
    player.animator.animations[4] = animation_new("falling", player.spriteSheet, 13, 13, 0.1f, FALSE);
    player.animator.currentAnimation = animation_get(&player.animator, "idle");
    player.boxcollider = collider_new_box(0, 0, 64, 64);

    /* Enemy Load Data */
    enemy.texture = load_texture(renderer, "content/hero.png");
    SDL_QueryTexture(enemy.texture, NULL, NULL, &enemy.srcR.w, &enemy.srcR.h);
    enemy.desR.w = enemy.desR.h = 16 * 4;
    enemy.position.x = enemy.desR.x = (config->window.width / 2 - enemy.desR.w /2);
    enemy.position.y = enemy.desR.y = (config->window.height - enemy.desR.h);
    enemy.spriteSheet = split_image(&enemy.srcR, 6, 5);
    enemy.flip = SDL_FLIP_HORIZONTAL;
    enemy.animator.animations[0] = animation_new("swimming", enemy.spriteSheet, 18, 23, 0.1f, TRUE);
    enemy.animator.currentAnimation = animation_get(&enemy.animator, "swimming");
    enemy.boxcollider = collider_new_box(enemy.desR.x, enemy.desR.y, 64, 64);

    while(!quit) {
        //Time loop
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;

        input_update(&inputState);

        player.velocity.x = 0;

        //Check inputs
        if(inputState.esc == PRESSED) quit = 1;
        if(inputState.left == PRESSED) player.velocity.x += -SPEED;
        if(inputState.right == PRESSED) player.velocity.x += SPEED;
        if(inputState.down == PRESSED){

            //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            //SDL_SetWindowSize(window, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
            //SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
            //SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        }
        if(inputState.up == PRESSED && isGround) {
            player.velocity.y = -420;
            isGround = 0;
        }

        //Update entity velocity
        player.position.x += player.velocity.x * deltaTime;
        player.velocity.y += GRAVITY * deltaTime;
        player.position.y += player.velocity.y * deltaTime;

        //Update box colliders
        collider_update_box(&player.boxcollider, player.position.x, player.position.y);
        collider_update_box(&enemy.boxcollider, enemy.position.x, enemy.position.y);

        //Collision detection
        box_collider_t minkowskiDifference = collider_minkowski_diff_box(enemy.boxcollider, player.boxcollider);
        if(collider_box_collision(player.boxcollider, enemy.boxcollider)){
            vector2_t penetrationVector = collider_penetration_vector_box(minkowskiDifference, VECTOR_ZERO);
            player.position.x += (int) penetrationVector.x;
            player.position.y += (int) penetrationVector.y;
            if(penetrationVector.y < 0){
                player.velocity.y = 0;
                isGround = 1;
            }
        }

        //Window collision detection
        if(player.position.x <= 0)
            player.position.x = 0;

        if(player.position.y <= 0)
            player.position.y = 0;

        if(player.position.x >= config->window.width - player.desR.w)
            player.position.x = config->window.width - player.desR.w;

        if(player.position.y >= config->window.height - player.desR.h) {
            player.position.y = config->window.height - player.desR.h;
            player.velocity.y = 0;
            isGround = 1;
        }

        //Apply animation
        if(player.velocity.x == 0 && player.velocity.y == 0) playerState = IDLE;
        if(player.velocity.x != 0){
            playerState = WALKING;
            player.flip = player.velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }
        if(player.velocity.y != 0) playerState = player.velocity.y < 0 ? JUMPING : FALLING;

        switch (playerState){
            case IDLE: animation_play(&player.animator, "idle"); break;
            case WALKING: animation_play(&player.animator, "walking"); break;
            case JUMPING: animation_play(&player.animator, "jumping"); break;
            case FALLING: animation_play(&player.animator, "falling"); break;
        }

        //Update entity position
        player.desR.x = (int) player.position.x;
        player.desR.y = (int) player.position.y;

        //Update colliders
        collider_update_box(&player.boxcollider, player.position.x, player.position.y);
        collider_update_box(&enemy.boxcollider, enemy.position.x, enemy.position.y);

        //Update animations
        animation_update(&player.srcR, &player.animator, deltaTime);
        animation_update(&enemy.srcR, &enemy.animator, deltaTime);

        //Set background color
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);

#if DEBUG
        //Box colliders render
        collider_render_box(player.boxcollider, renderer, (color_t) {0, 0, 255, 255});
        collider_render_box(enemy.boxcollider, renderer, (color_t) {0, 0, 255, 255});
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