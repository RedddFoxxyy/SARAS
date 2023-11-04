#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <time.h>

SDL_Texture* playerTexture = NULL;
SDL_Texture* startTexture = NULL;
SDL_Texture* endTexture = NULL;
SDL_Texture* endscreenTexture = NULL;
SDL_Texture* startscreenTexture = NULL;

// Define constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int MAZE_SIZE = 25; // Adjust based on your maze sizes

void delay(int time) {
    // this function will create a pause in the runtime process for the number of milliseconds that is given in 'time'
    long pause;
    clock_t time1, time2;

    pause = time * (CLOCKS_PER_SEC / 1000);
    time1 = time2 = clock();

    // the clock() returns back the number of ticks till it has been called
    while (time2 - time1 < pause) {
        time2 = clock();
    }
}

int maze[1000][1000] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1},
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
        {1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,3,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// Player's position
int playerX = 2;
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

// Create a function to display the endscreen image.
void displayEndscreen(SDL_Renderer* renderer) {
    if (endscreenTexture == NULL) {
        printf("Endscreen texture is NULL!\n");
        return;
    }

    // Clear the renderer before drawing the endscreen texture.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect endscreenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, endscreenTexture, NULL, &endscreenRect);

    // Update the renderer
    SDL_RenderPresent(renderer);
}

// Create a function to display the startscreen image.
void displayStartscreen(SDL_Renderer* renderer) {
    if (startscreenTexture == NULL) {
        printf("Endscreen texture is NULL!\n");
        return;
    }

    // Clear the renderer before drawing the startscreen texture.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect startscreenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, startscreenTexture, NULL, &startscreenRect);

    // Update the renderer
    SDL_RenderPresent(renderer);

    return;
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

    // Load the startscreen image into a texture.
    startscreenTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("C:\\Users\\SUYOG\\Documents\\GitHub\\SARAS\\sprites\\startscreen.bmp"));
    if (startscreenTexture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return 6;
    }

    // Load the endscreen image into a texture.
    endscreenTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("C:\\Users\\SUYOG\\Documents\\GitHub\\SARAS\\sprites\\endscreen.bmp"));
    if (endscreenTexture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return 5;
    }

    // Load the start and end images
    startTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("C:\\Users\\SUYOG\\Documents\\GitHub\\SARAS\\sprites\\start.bmp"));
    endTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("C:\\Users\\SUYOG\\Documents\\GitHub\\SARAS\\sprites\\end.bmp"));
    if (startTexture == NULL || endTexture == NULL) {
        printf("Could not load start or end image! SDL_Error: %s\n", SDL_GetError());
        return 4;
    }

    if (!loadPlayerTexture(renderer)) {
        return 4;
    }

    bool quit = false;
    SDL_Event e;

    displayStartscreen(renderer);

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
                 SDL_Rect tileRect = {x * (SCREEN_WIDTH / MAZE_SIZE), y * (SCREEN_HEIGHT / MAZE_SIZE),
                                      SCREEN_WIDTH / MAZE_SIZE, SCREEN_HEIGHT / MAZE_SIZE};
                 if (maze[y][x] == 1) {
                     SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
                     SDL_RenderFillRect(renderer, &tileRect);
                 } else if (maze[y][x] == 2) {
                     SDL_RenderCopy(renderer, startTexture, NULL, &tileRect);
                 } else if (maze[y][x] == 3) {
                     SDL_RenderCopy(renderer, endTexture, NULL, &tileRect);
                 }
             }
         }


         // Draw the player
         SDL_Rect playerRect = {playerX * (SCREEN_WIDTH / MAZE_SIZE), playerY * (SCREEN_HEIGHT / MAZE_SIZE),
                                SCREEN_WIDTH / MAZE_SIZE, SCREEN_HEIGHT / MAZE_SIZE};
         SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);


         // Update the renderer
         SDL_RenderPresent(renderer);

         if (playerX == 22 && playerY == 23) {
             printf("Displaying end screen\n");
             displayEndscreen(renderer);
             SDL_DestroyTexture(startTexture);
             SDL_DestroyTexture(endTexture);
             SDL_DestroyTexture(playerTexture);
             delay(5000);
             SDL_DestroyTexture(endscreenTexture);
             SDL_DestroyRenderer(renderer);
             SDL_DestroyWindow(window);
             SDL_Quit();
             break;
         }


    }
    SDL_DestroyTexture(endscreenTexture);
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(endTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}