#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <malloc.h>

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

typedef enum bool {
    FALSE = 0,
    TRUE = 1
} bool_t;

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
    SDL_Surface *sprite;
    SDL_Rect srcR;
    SDL_Rect destR;
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

SDL_Texture *loadTexture(SDL_Renderer* renderer, SDL_Surface* surface){
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
};

SDL_Rect *splitImage(SDL_Rect *rect, int column, int row) {

    SDL_Rect *image = (SDL_Rect *) malloc((column * row) * sizeof(SDL_Rect));
    int splitWidth = rect->w / column;
    int splitHeight = rect->h / row;
    int i = 0;
    printf("SplitWidth: %d SplitHeight: %d\n", splitWidth, splitHeight);

    for (int y = 0; y < row; ++y) {
        for (int x = 0; x < column; ++x) {
            (image + i)->x = splitWidth * x;
            (image + i)->y = splitHeight * y;
            (image + i)->w = splitWidth;
            (image + i)->h = splitHeight;
            printf("SplitImage: %d PositionX: %d PositionY: %d\n", i, (image + i)->x, (image + i)->y);
            i++;
        }
    }

    return image;
}

int main(int argc, char* args[]){
    char gameTitle[] = "Game Window";
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;

    int prevTime = 0;
    int currentTime = 0;
    float deltaTime = 0;
    float frameTime = 0;
    int close_requested = 0;

    input_state_t inputState = initInput();
    entity_t player;

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    player.sprite = IMG_Load("content/oldHero.png");
    texture = loadTexture(renderer, player.sprite);

    int i = 0;

    SDL_QueryTexture(texture, NULL, NULL, &player.srcR.w, &player.srcR.h);
    SDL_Rect *spriteSheet = splitImage(&player.srcR, 6, 1);
    player.srcR = spriteSheet[i];

    player.destR.x = 0;
    player.destR.y = 0;
    player.destR.w = 16 * 4;
    player.destR.h = 18 * 4;

    player.position.x = (SCREEN_WIDTH - player.destR.w)/2;
    player.position.y = (SCREEN_HEIGHT - player.destR.h)/2;

    while(!close_requested) {

        //Time loop
        prevTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - prevTime) / 1000.0f;
        frameTime += deltaTime;

        updateInput(&inputState);

        player.velocity.x = player.velocity.y = 0;

        //Check inputs
        if(inputState.esc == PRESSED) close_requested = 1;
        if(inputState.up == PRESSED) player.velocity.y += -SPEED;
        if(inputState.down == PRESSED) player.velocity.y += SPEED;
        if(inputState.left == PRESSED) player.velocity.x += -SPEED;
        if(inputState.right == PRESSED) player.velocity.x += SPEED;

        //Update entity velocity
        player.position.x += player.velocity.x * deltaTime;
        player.position.y += player.velocity.y * deltaTime;

        //Collision detection
        if(player.position.x <= 0) player.position.x = 0;
        if(player.position.y <= 0) player.position.y = 0;
        if(player.position.x >= SCREEN_WIDTH - player.destR.w) {
            player.position.x = SCREEN_WIDTH - player.destR.w;
        }
        if(player.position.y >= SCREEN_HEIGHT - player.destR.h) {
            player.position.y = SCREEN_HEIGHT - player.destR.h;
        }

        if(frameTime >= 0.1f){
            frameTime = 0;
            player.srcR = spriteSheet[i];
            if(i >= 6){
                i = 0;
                player.srcR = spriteSheet[i];
            }
            i++;
        }

        //Update entity position
        player.destR.y = (int) player.position.y;
        player.destR.x = (int) player.position.x;

        //Entity renders
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &player.srcR, &player.destR);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}