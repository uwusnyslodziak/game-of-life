#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int WIDTH = 1900, HEIGHT = 1100, FPS = 100;
const int SIZE = 40, INITSPEED = 10;
int SPEED = INITSPEED;
const int frameDelay = 1000 / FPS;
const char *title = "Game of Life";
SDL_Window *window = SDL_CreateWindow(title, 0, 0, WIDTH, HEIGHT, 0);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

struct Color { int r, g, b; };
enum Colors {
    BLACK,
    WHITE,
    GRID
};
Color color[] = {
    { 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF },
    { 0x7F, 0x7F, 0x7F }
};
void SetColor(Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
}

const int CELLSIZE = std::min(WIDTH, HEIGHT)/SIZE;
const int W = WIDTH/CELLSIZE, H = HEIGHT/CELLSIZE;

bool board[2][W][H];

bool currBoard = true;
void nextBoard(){
    for (int x=0; x<W; x++){
        for (int y=0; y<H; y++){
            int n = 
                board[currBoard][(x-1+W)%W][(y-1+H)%H] +
                board[currBoard][(x-1+W)%W][(y)%H] +
                board[currBoard][(x-1+W)%W][(y+1)%H] +
                board[currBoard][(x)%W][(y-1+H)%H] +
                board[currBoard][(x)%W][(y+1)%H] +
                board[currBoard][(x+1)%W][(y-1+H)%H] +
                board[currBoard][(x+1)%W][(y)%H] +
                board[currBoard][(x+1)%W][(y+1)%H];
            if (board[currBoard][x][y]){
                if (n == 2 | n == 3){
                    board[currBoard^1][x][y] = true;
                }
                else {
                    board[currBoard^1][x][y] = false;
                }
            }
            else {
                if (n == 3){
                    board[currBoard^1][x][y] = true;
                }
                else {
                    board[currBoard^1][x][y] = false;
                }
            }
        }
    }
    currBoard ^= 1;
}
void modifyCell(int x, int y, bool addCell = true){
    if (x < 0 | x >= WIDTH | y < 0 | y >= HEIGHT) return;
    x /= CELLSIZE; y /= CELLSIZE;
    board[currBoard][x][y] = addCell;
}
bool invertedColors = false, showGrid = false;
void showBoard(){
    for (int x=0; x<W; x++){
        for (int y=0; y<H; y++){
            if (board[currBoard][x][y]){
                SetColor(color[invertedColors^WHITE]);
            }
            else {
                SetColor(color[invertedColors^BLACK]);
            }
            SDL_Rect cell = {
                x*CELLSIZE, y*CELLSIZE,
                CELLSIZE+1, CELLSIZE+1
            };
            SDL_RenderFillRect(renderer, &cell);
            if (showGrid){
                SetColor(color[GRID]);
                SDL_RenderDrawRect(renderer, &cell);
            }

        }
    }
}
void clearBoard(){
    for (int x=0; x<W; x++){
        for (int y=0; y<H; y++){
            board[0][x][y] = board[1][x][y] = false;
        }
    }
}

int main(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if(!window) return 1;
    if(!renderer) return 1;

    int frameCount = 0;
    bool running = true, simulate = true;
    while(running){
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0){
            if (e.type == SDL_QUIT){
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym){
                    case ' ':
                        std::cout << "play/pause\n";
                        simulate ^= 1;
                        frameCount = 0;
                        break;
                    case 'c':
                        std::cout << "clear\n";
                        clearBoard();
                        break;
                    case 'n':
                        if (!simulate){
                            std::cout << "advance\n";
                            nextBoard();
                        }
                        break;
                    case 'g':
                        std::cout << "toggle grid\n";
                        showGrid ^= 1;
                        break;
                    case 'i':
                        std::cout << "invert colors\n";
                        invertedColors ^= 1;
                        break;
                    case 'r':
                        std::cout << "reset settings\n";
                        showGrid = false;
                        invertedColors = false;
                        SPEED = INITSPEED;
                        break;
                    case SDLK_UP:
                        SPEED = std::max(SPEED-1, 1);
                        break;
                    case SDLK_DOWN:
                        SPEED = std::min(SPEED+1, FPS);
                        break;
                    case 'q':
                        std::cout << "quit\n";
                        running = false;
                }
            }
        }

        SetColor(color[GRID]);
        SDL_RenderClear(renderer);

        if (!frameCount && simulate){
            nextBoard();
        }
        int mx, my;
        if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_LMASK){
            modifyCell(mx, my);
        }
        if (SDL_GetMouseState(&mx, &my) & SDL_BUTTON_RMASK){
            modifyCell(mx, my, false);
        }
        showBoard();

        SDL_RenderPresent(renderer);
        frameCount = (frameCount+1)%SPEED;

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
