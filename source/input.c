#include <SDL.h>
#include <input.h>

void input_update(input_state_t *input) {

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        SDL_Scancode scanCode = event.key.keysym.scancode;

        switch(event.type) {
            case SDL_KEYDOWN:
                if(scanCode == SDL_SCANCODE_ESCAPE) input->esc = PRESSED;
                if(scanCode == SDL_SCANCODE_UP) input->up = PRESSED;
                if(scanCode == SDL_SCANCODE_DOWN) input->down = PRESSED;
                if(scanCode == SDL_SCANCODE_LEFT) input->left = PRESSED;
                if(scanCode == SDL_SCANCODE_RIGHT) input->right = PRESSED;
                break;
            case SDL_KEYUP:
                if(scanCode == SDL_SCANCODE_ESCAPE) input->esc = RELEASED;
                if(scanCode == SDL_SCANCODE_UP) input->up = RELEASED;
                if(scanCode == SDL_SCANCODE_DOWN) input->down = RELEASED;
                if(scanCode == SDL_SCANCODE_LEFT) input->left = RELEASED;
                if(scanCode == SDL_SCANCODE_RIGHT) input->right = RELEASED;
                break;
        }
    }
}