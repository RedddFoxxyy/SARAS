#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <windows.h>

SDL_Texture* playerTexture = NULL;
SDL_Texture* startTexture = NULL;
SDL_Texture* endTexture = NULL;
SDL_Texture* endscreenTexture = NULL;
SDL_Texture* startscreenTexture = NULL;
SDL_Texture* gameoverTexture = NULL;

// The audio data structure
typedef struct {
    Uint8* buffer;
    Uint32 length;
    SDL_AudioStream* stream;
} AudioData;

// Define constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int MAZE_SIZE = 25; // Adjust based on your maze sizes
const Uint32 GAME_TIME = 45000; // Game time in milliseconds

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

// The audio callback function
void audio_callback(void* userdata, Uint8* stream, int len) {
    AudioData* audio_data = (AudioData*)userdata;
    while (len > 0) {
        int audio_len = SDL_AudioStreamGet(audio_data->stream, stream, len);
        if (audio_len == 0) {
            // We've reached the end of the stream, stop the audio
            SDL_ClearQueuedAudio(1);
            break;
        }
        len -= audio_len;
        stream += audio_len;
    }
}

char* GetExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    size_t pos = strrchr(buffer, '\\') - buffer + 1;
    char* result = (char*) malloc(pos + 1);
    strncpy(result, buffer, pos);
    result[pos] = '\0';
    return result;
}

char* GetFilePath(const char* filename) {
    char* executablePath = GetExecutablePath();
    size_t len = strlen(executablePath) + strlen(filename) + 2;
    char* result = (char*) malloc(len);
    strcpy(result, executablePath);
    strcat(result, "\\");
    strcat(result, filename);
    free(executablePath);
    return result;
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

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

bool loadPlayerTexture(SDL_Renderer* renderer) {
    char* filePath = GetFilePath("suyog.bmp");
    SDL_Surface* loadedSurface = SDL_LoadBMP(filePath);
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
        printf("Startscreen texture is NULL!\n");
        return;
    }

    // Clear the renderer before drawing the startscreen texture.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect startscreenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, startscreenTexture, NULL, &startscreenRect);

    // Update the renderer
    SDL_RenderPresent(renderer);
}

// Create a function to display the gameover image.
void displaygameoverscreen(SDL_Renderer* renderer) {
    if (gameoverTexture == NULL) {
        printf("Gameover texture is NULL!\n");
        return;
    }

    // Clear the renderer before drawing the gameover texture.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect gameoverRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, gameoverTexture, NULL, &gameoverRect);

    // Update the renderer
    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    if (!initialize()) {
        return 1;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load the WAV file
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8* wav_buffer;
    char* musicPath = GetFilePath("03-Title-Screen.wav");
    if (SDL_LoadWAV(musicPath, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        printf("Could not open background.wav! SDL_Error: %s\n", SDL_GetError());
        return 2;
    }

    // Create an audio stream
    SDL_AudioStream* audio_stream = SDL_NewAudioStream(wav_spec.format, wav_spec.channels, wav_spec.freq, wav_spec.format, wav_spec.channels, wav_spec.freq);
    SDL_AudioStreamPut(audio_stream, wav_buffer, wav_length);

    // Create the audio data structure
    AudioData audio_data = { wav_buffer, wav_length, audio_stream };

    // Set the audio callback
    wav_spec.callback = audio_callback;
    wav_spec.userdata = &audio_data;

    // Open the audio device
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
    if (device == 0) {
        printf("Could not open audio device! SDL_Error: %s\n", SDL_GetError());
        return 3;
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

    // Load a font
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
    printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    return 4;
    }

    // Create a surface for the timer text
    SDL_Color color = { 255, 255, 255, 255 }; // White color
    SDL_Surface* timer_surface = TTF_RenderText_Solid(font, "45", color);
    if (timer_surface == NULL) {
    printf("Failed to render text! SDL_ttf Error: %s\n", TTF_GetError());
    return 5;
    }

    // Create a texture from the surface
    SDL_Texture* timer_texture = SDL_CreateTextureFromSurface(renderer, timer_surface);
    if (timer_texture == NULL) {
    printf("Failed to create texture from surface! SDL Error: %s\n", SDL_GetError());
    return 6;
    }


    // Load the startscreen image into a texture.
    char* startscreenPath = GetFilePath("startscreen2_1.bmp");
    startscreenTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(startscreenPath));
    if (startscreenTexture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return 6;
    }

    // Load the gameoverscreen image into a texture.
    char* gameoverscreenPath = GetFilePath("Game-Over_1.bmp");
    gameoverTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(gameoverscreenPath));
    if (gameoverTexture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return 7;
    }

    // Load the endscreen image into a texture.
    char* endscreenPath = GetFilePath("You-Won.bmp");
    endscreenTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(endscreenPath));
    if (endscreenTexture == NULL) {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return 5;
    }

    // Load the start and end images
    char* startimagePath = GetFilePath("start.bmp");
    startTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(startimagePath));
    char* endimagePath = GetFilePath("end.bmp");
    endTexture = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(endimagePath));
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

    // Get user input to start the game
    SDL_Event f;
    bool started = false;
    while (!started) {
        while (SDL_PollEvent(&f) != 0) {
            if (f.type == SDL_QUIT) {
                quit = true;
            }

            if (f.type == SDL_KEYDOWN) {
                if (f.key.keysym.sym == SDLK_RETURN) {
                    started = true;
                }
            }
        }
    }

    // Start playing audio
    SDL_PauseAudioDevice(device, 0);

    // Get the start time
    Uint32 start_time = SDL_GetTicks();

     while (!quit) {

         if (SDL_GetTicks() - start_time >= GAME_TIME) {
             SDL_PauseAudioDevice(device, 1);
             displaygameoverscreen(renderer);
             printf("Game over! Time's up!\n");
             SDL_DestroyTexture(startTexture);
             SDL_DestroyTexture(endTexture);
             SDL_DestroyTexture(playerTexture);
             delay(5000);
             SDL_DestroyTexture(endscreenTexture);
             SDL_DestroyRenderer(renderer);
             SDL_DestroyWindow(window);
             SDL_CloseAudioDevice(device);
             SDL_FreeWAV(wav_buffer);
             SDL_FreeAudioStream(audio_stream);
             SDL_Quit();
             break;
         }

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
                     SDL_SetRenderDrawColor(renderer, 80, 160, 250, 255);
                     SDL_RenderFillRect(renderer, &tileRect);
                 } else if (maze[y][x] == 2) {
                     SDL_RenderCopy(renderer, startTexture, NULL, &tileRect);
                 } else if (maze[y][x] == 3) {
                     SDL_RenderCopy(renderer, endTexture, NULL, &tileRect);
                 }
             }
         }
         // Update the timer text
         char timer_text[3];
         sprintf(timer_text, "%d", (GAME_TIME - (SDL_GetTicks() - start_time)) / 1000);
         SDL_FreeSurface(timer_surface);
         SDL_DestroyTexture(timer_texture);
         timer_surface = TTF_RenderText_Solid(font, timer_text, color);
         timer_texture = SDL_CreateTextureFromSurface(renderer, timer_surface);

         // Render the timer text
         SDL_Rect timer_rect = { SCREEN_WIDTH - 50, 10, 40, 30 }; // Adjust based on your needs
         SDL_RenderCopy(renderer, timer_texture, NULL, &timer_rect);

         // Draw the player
         SDL_Rect playerRect = {playerX * (SCREEN_WIDTH / MAZE_SIZE), playerY * (SCREEN_HEIGHT / MAZE_SIZE),
                                SCREEN_WIDTH / MAZE_SIZE, SCREEN_HEIGHT / MAZE_SIZE};
         SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);


         // Update the renderer
         SDL_RenderPresent(renderer);

         if (playerX == 22 && playerY == 23) {
             printf("Displaying end screen\n");
             delay(200);
             displayEndscreen(renderer);
             SDL_DestroyTexture(startTexture);
             SDL_DestroyTexture(endTexture);
             SDL_DestroyTexture(playerTexture);
             delay(5000);
             SDL_DestroyTexture(endscreenTexture);
             SDL_DestroyRenderer(renderer);
             SDL_DestroyWindow(window);
             SDL_CloseAudioDevice(device);
             SDL_FreeWAV(wav_buffer);
             SDL_FreeAudioStream(audio_stream);
             SDL_Quit();
             break;
         }


    }

    // Clean up
    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(wav_buffer);
    SDL_FreeAudioStream(audio_stream);
    SDL_DestroyTexture(endscreenTexture);
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(endTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
