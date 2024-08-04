#include <iostream>
#include <SDL2/sdl.h>
#include <fmt/core.h>
#include <physicsEngine.hpp>
#include <utility>

using namespace std;

//Screen Resolution
const int screenHeight = 600;
const int screenWidth = 800;

int main(int argc, char *argv[]){
  
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window* window = SDL_CreateWindow("Collision Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  bool running = true;
  SDL_Event event;

  while (running){
  
    if (SDL_PollEvent(&event)) {
      /* an event was found */
      switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
          running = false;
          break;
      }
    }

    /* update the screen */
    SDL_RenderPresent(renderer);
  }


  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
