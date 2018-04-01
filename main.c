#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPEED = 300;
const int FPS = 60;
const int FRAME_DELAY = 1000/60;

typedef enum KEY_STATE {
    NORMAL,
    PRESSED,
    RELEASED
} KEY_STATE_T;

typedef struct input_state {
    KEY_STATE_T up;
    KEY_STATE_T down;
    KEY_STATE_T left;
    KEY_STATE_T right;
    KEY_STATE_T esc;
} input_state_t;

typedef struct vector2 {
    float x, y;
} vector2_t;

typedef struct entity {
    vector2_t position;
    vector2_t velocity;
    SDL_Surface* sprite;
} entity_t;

input_state_t initInput(){
    input_state_t inputState =  {NORMAL, NORMAL, NORMAL, NORMAL, NORMAL};
    return inputState;
}

void updateInput(input_state_t* inputState) {

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        SDL_Scancode scanCode = event.key.keysym.scancode;

        switch(event.type) {
            case SDL_KEYDOWN:
                if(scanCode == SDL_SCANCODE_ESCAPE) inputState->esc = PRESSED;
                if(scanCode == SDL_SCANCODE_UP) inputState->up = PRESSED;
                if(scanCode == SDL_SCANCODE_DOWN) inputState->down = PRESSED;
                if(scanCode == SDL_SCANCODE_LEFT) inputState->left = PRESSED;
                if(scanCode == SDL_SCANCODE_RIGHT) inputState->right = PRESSED;
                break;
            case SDL_KEYUP:
                if(scanCode == SDL_SCANCODE_ESCAPE) inputState->esc = RELEASED;
                if(scanCode == SDL_SCANCODE_UP) inputState->up = RELEASED;
                if(scanCode == SDL_SCANCODE_DOWN) inputState->down = RELEASED;
                if(scanCode == SDL_SCANCODE_LEFT) inputState->left = RELEASED;
                if(scanCode == SDL_SCANCODE_RIGHT) inputState->right = RELEASED;
                break;
        }
    }
}

void clean(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Window* window){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, SDL_Surface* surface){
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
};

int main(int argc, char* args[]){

    char gameTitle[] = "Game Window";
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect rect;

    input_state_t inputState = initInput();
    entity_t player;

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    player.sprite = IMG_Load("content/image.png");
    texture = loadTexture(renderer, player.sprite);

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    rect.w /= 4;
    rect.h /= 4;

    player.position.x = (SCREEN_WIDTH - rect.w)/2;
    player.position.y = (SCREEN_HEIGHT - rect.h)/2;

    uint32_t frameStart = SDL_GetTicks();
    int frameTime, elapsedTime;
    int close_requested = 0;

    while(!close_requested) {
        frameTime = SDL_GetTicks();
        elapsedTime = frameTime - frameStart;
        frameStart = frameTime;
        updateInput(&inputState);

        player.velocity.x = player.velocity.y = 0;

        //Check inputs
        if(inputState.esc == PRESSED) close_requested = 1;
        if(inputState.up == PRESSED) player.velocity.y += -SPEED;
        if(inputState.down == PRESSED) player.velocity.y += SPEED;
        if(inputState.left == PRESSED) player.velocity.x += -SPEED;
        if(inputState.right == PRESSED) player.velocity.x += SPEED;

        //Update entity velocity
        player.position.x += player.velocity.x / 60;
        player.position.y += player.velocity.y / 60;

        //Collision detection
        if(player.position.x <= 0) player.position.x = 0;
        if(player.position.y <= 0) player.position.y = 0;
        if(player.position.x >= SCREEN_WIDTH - rect.w) player.position.x = SCREEN_WIDTH - rect.w;
        if(player.position.y >= SCREEN_HEIGHT - rect.h) player.position.y = SCREEN_HEIGHT - rect.h;

        //Update entity position
        rect.y = (int) player.position.y;
        rect.x = (int) player.position.x;

        //Entity renders
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);

        if(frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - elapsedTime);
    }

    clean(texture, renderer, window);

    return 0;
}