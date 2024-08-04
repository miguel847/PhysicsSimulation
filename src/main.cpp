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
  
  SDL_Window* window = SDL_CreateWindow("Collision Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
