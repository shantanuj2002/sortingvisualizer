#include"inc/SDL.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#undef main
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

const int ARRAY_SIZE = 15;
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

void drawArray(int pivotIdx = -1, int leftIdx = -1, int rightIdx = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        // Set color for the current element
        if (i == pivotIdx) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for pivot element
        } else if (i == leftIdx || i == rightIdx) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for left and right elements
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for other elements
        }

        SDL_Rect rect = {i * 60, SCREEN_HEIGHT - array[i], 40, array[i]};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

int partition(int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (array[j] < pivot) {
            i++;
            std::swap(array[i], array[j]);
            drawArray(high, i, j); // Highlight pivot, left, and right elements
            SDL_Delay(200); // Delay for visualization
        }
    }
    std::swap(array[i + 1], array[high]);
    drawArray(); // Redraw array
    SDL_Delay(200); // Delay for visualization
    return i + 1;
}

void quickSort(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);

        quickSort(low, pi - 1);
        quickSort(pi + 1, high);
    }
}

int main() {
    if (!initSDL()) {
        return 1;
    }

    generateArray(); // Generate random array

    bool quit = false;
    SDL_Event e;

    quickSort(0, ARRAY_SIZE - 1); // Perform quick sort

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    closeSDL();
    return 0;
}
