#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int WIDTH = 960, HEIGHT = 720, FPS = 60, SIZE = 20;
const int frameDelay = 1000 / FPS;
const char *title = "Follower";
SDL_Window *window = SDL_CreateWindow(title, 0, 0, WIDTH, HEIGHT, 0);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

struct Color { int r, g, b; };
enum Colors {
    BLACK,
    WHITE
};
Color color[] = {
    { 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF }};
void SetColor(Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
}

const int CELLSIZE = std::min(WIDTH, HEIGHT)/SIZE;
const int W = WIDTH/CELLSIZE, H = HEIGHT/CELLSIZE;

bool board[2][W][H];

void addCell(int x, int y){
    std::cout << x << " " << y << std::endl;
}

int main(){

    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if(!window) return 1;
    if(!renderer) return 1;

    bool running = true;
    while(running){
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                running = false;
            }
        }

        SetColor(color[BLACK]);
        SDL_RenderClear(renderer);

        int x, y;
        SDL_GetMouseState(&x, &y);
        addCell(x, y);

        SDL_RenderPresent(renderer);

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
