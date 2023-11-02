#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Texture* playerTexture = NULL;

// Define constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int MAZE_SIZE = 25; // Adjust based on your maze size


int maze[1000][1000] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1},
        {1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,0,1},
        {1,1,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1},
        {1,1,1,0,1,0,1,0,0,0,1,0,1,1,1,0,1,0,1,0,1,0,0,0,1},
        {1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1},
        {1,1,1,1,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,1,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1},
        {1,1,0,1,1,1,1,0,0,1,0,1,0,1,0,1,1,0,1,1,1,0,1,0,1},
        {1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1},
        {1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1},
        {1,1,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,0,0,1},
        {1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,1,0,1,0,1},
        {1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1},
        {1,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1},
        {1,1,0,1,0,0,0,1,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1},
        {1,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1},
        {1,1,0,0,1,0,0,0,0,1,1,0,1,1,1,0,1,1,1,1,0,1,0,1,1},
        {1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,1,1},
        {1,1,1,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,0,0,0,1,1},
        {1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// Player's position
int playerX = 1;
int playerY = 1;

// Initialize SDL
bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool loadPlayerTexture(SDL_Renderer* renderer) {
    SDL_Surface* loadedSurface = SDL_LoadBMP("D:\\Developement\\Game-Project\\saras-dev\\cute-duck-png_5f81db64377e1.bmp");
    if (loadedSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    playerTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (playerTexture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_FreeSurface(loadedSurface);
    return true;
}


int main(int argc, char* args[]) {
    if (!initialize()) {
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SARAS The Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 2;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 3;
    }

    if (!loadPlayerTexture(renderer)) {
        return 4;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                // Handle user input to move the player
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (maze[playerY - 1][playerX] == 0) {
                            playerY--;
                        }
                        break;
                    case SDLK_DOWN:
                        if (maze[playerY + 1][playerX] == 0) {
                            playerY++;
                        }
                        break;
                    case SDLK_LEFT:
                        if (maze[playerY][playerX - 1] == 0) {
                            playerX--;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (maze[playerY][playerX + 1] == 0) {
                            playerX++;
                        }
                        break;
                }
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the maze
        for (int y = 0; y < MAZE_SIZE; y++) {
            for (int x = 0; x < MAZE_SIZE; x++) {
                if (maze[y][x] == 1) {
                    SDL_Rect wallRect = { x * (SCREEN_WIDTH / MAZE_SIZE), y * (SCREEN_HEIGHT / MAZE_SIZE), SCREEN_WIDTH / MAZE_SIZE, SCREEN_HEIGHT / MAZE_SIZE };
                    SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
                    SDL_RenderFillRect(renderer, &wallRect);
                }
            }
        }

        // Draw the player
        SDL_Rect playerRect = { playerX * (SCREEN_WIDTH / MAZE_SIZE), playerY * (SCREEN_HEIGHT / MAZE_SIZE), SCREEN_WIDTH / MAZE_SIZE, SCREEN_HEIGHT / MAZE_SIZE };
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);


        // Update the renderer
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}