#include <SDL.h>
#include <input.h>

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