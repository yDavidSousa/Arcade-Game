#include <stdio.h>
#include <SDL.h>
#include <input.h>
#include <graphic.h>
#include <collider.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPEED = 300;
const int FPS = 60;

typedef struct vector2 {
    float x, y;
} vector2_t;

typedef struct entity {
    vector2_t position;
    vector2_t velocity;
    SDL_Rect srcR;
    SDL_Rect destR;
    SDL_Texture* texture;
} entity_t;

int main(int argc, char* args[]){
    char gameTitle[] = "Game Window";
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int prevTime = 0;
    int currentTime = 0;
    float deltaTime = 0;
    float frameTime = 0;
    int close_requested = 0;
    int i = 0;

    entity_t player, enemy;
    input_state_t inputState = {NORMAL, NORMAL, NORMAL, NORMAL, NORMAL};

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //Player Data
    player.texture = loadTexture(renderer, "content/oldHero.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.srcR.w, &player.srcR.h);
    SDL_Rect *spriteSheet = splitImage(&player.srcR, 6, 1);
    player.srcR = spriteSheet[i];
    player.destR.w = 16 * 4;
    player.destR.h = 18 * 4;
    player.position.x = 40;
    player.position.y = (SCREEN_HEIGHT - player.destR.h);
    SDL_RenderDrawRect(renderer, &enemy.destR);

    //Enemy Data
    enemy.texture = loadTexture(renderer, "content/image.png");
    SDL_QueryTexture(enemy.texture, NULL, NULL, &enemy.destR.w, &enemy.destR.h);
    enemy.destR.w /= 4;
    enemy.destR.h /= 4;
    enemy.destR.x = (SCREEN_WIDTH - enemy.destR.w);
    enemy.destR.y = (SCREEN_HEIGHT - enemy.destR.h);

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

        //Collision detection
        SDL_SetTextureColorMod(player.texture, 255, 255, 255);
        if(boxCollision(player.destR, enemy.destR))
            SDL_SetTextureColorMod(player.texture, 255, 0, 0);

        //Update entity velocity
        player.position.x += player.velocity.x * deltaTime;
        player.position.y += player.velocity.y * deltaTime;

        //Window collision detection
        if(player.position.x <= 0) player.position.x = 0;
        if(player.position.y <= 0) player.position.y = 0;
        if(player.position.x >= SCREEN_WIDTH - player.destR.w) player.position.x = SCREEN_WIDTH - player.destR.w;
        if(player.position.y >= SCREEN_HEIGHT - player.destR.h) player.position.y = SCREEN_HEIGHT - player.destR.h;

        //Apply animation
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
        player.destR.x = (int) player.position.x;
        player.destR.y = (int) player.position.y;

        //Set background color
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear(renderer);
#if DEBUG
        //Box collider render
        SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderDrawRect( renderer, &enemy.destR );
        SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
        SDL_RenderDrawRect( renderer, &player.destR );
#endif
        //Entity renders
        SDL_RenderCopy(renderer, enemy.texture, NULL, &enemy.destR); // Depth 0
        SDL_RenderCopy(renderer, player.texture, &player.srcR, &player.destR); // Depth 1
        SDL_RenderPresent(renderer);

        if(currentTime < FPS)
            SDL_Delay(currentTime - prevTime);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyTexture(enemy.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}