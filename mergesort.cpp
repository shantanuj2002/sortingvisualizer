#include"inc/SDL.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
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

void drawArray(const std::vector<int>& sortedIndices) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        // Set color for the current element
        bool isSorted = false;
        for (int idx : sortedIndices) {
            if (i == idx) {
                isSorted = true;
                break;
            }
        }
        if (isSorted) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for sorted elements
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for other elements
        }

        SDL_Rect rect = {i * 60, SCREEN_HEIGHT - array[i], 40, array[i]};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);
}

void merge(int low, int mid, int high, std::vector<int>& sortedIndices) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = array[low + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
        drawArray(sortedIndices);
        SDL_Delay(200);
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
        drawArray(sortedIndices);
        SDL_Delay(200);
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
        drawArray(sortedIndices);
        SDL_Delay(200);
    }
}

void mergeSort(int low, int high, std::vector<int>& sortedIndices) {
    if (low < high) {
        int mid = low + (high - low) / 2;

        mergeSort(low, mid, sortedIndices);
        mergeSort(mid + 1, high, sortedIndices);

        merge(low, mid, high, sortedIndices);
    }
}

int main() {
    if (!initSDL()) {
        return 1;
    }

    generateArray(); // Generate random array

    bool quit = false;
    SDL_Event e;

    std::vector<int> sortedIndices;
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        sortedIndices.push_back(i);
    }

    mergeSort(0, ARRAY_SIZE - 1, sortedIndices); // Perform merge sort

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
