#include"inc/SDL.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#undef main
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;

const int ARRAY_SIZE = 7;
int array[ARRAY_SIZE];

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Sorting Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void generateArray() {
    std::srand(std::time(nullptr));
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = std::rand() % SCREEN_HEIGHT;
    }
}

void drawArray(int comparedIdx = -1, int swapIdx = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        // Set color for the current element
        if (i == comparedIdx) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for compared elements
        } else if (i == swapIdx) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for swap elements
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for other elements
        }

        SDL_Rect rect = {i * 80, SCREEN_HEIGHT - array[i], 60, array[i]};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

void bubbleSort() {
    bool sorted = false;
    while (!sorted) {
        sorted = true; // Assume the array is sorted

        for (int i = 0; i < ARRAY_SIZE - 1; ++i) {
            drawArray(i, i + 1); // Highlight elements being compared
            SDL_Delay(100); // Delay for visualization

            if (array[i] > array[i + 1]) {
                std::swap(array[i], array[i + 1]);
                sorted = false; // If we make a swap, then the array is not sorted yet
                drawArray(i, i + 1); // Highlight elements being swapped
                SDL_Delay(200); // Delay for visualization
            }
        }
    }
}

int main() {
    if (!initSDL()) {
        return 1;
    }

    generateArray(); // Generate random array

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        bubbleSort();
        quit = true; // Stop the program after sorting
    }

    closeSDL();
    return 0;
}
