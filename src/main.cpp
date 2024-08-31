#include <iostream>
#include <SDL2/sdl.h>
#include <fmt/core.h>
#include <physicsEngine.hpp>
#include <vector>
#include <algorithm>
#include <utility>
#include <execution>

using namespace std;

//Screen Resolution
const int screenHeight = 600;
const int screenWidth = 800;

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius, SDL_Color color)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);  
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y)
  {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

    if (error <= 0)
    {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0)
    {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
}

void DrawBodies(SDL_Renderer* renderer, vector<std::unique_ptr<body>>& bodyArray, SDL_Color color){
  for(int i = 0; i < bodyArray.size(); ++i){
    auto body = bodyArray[i].get();
    DrawCircle(renderer, body->position.x+(screenWidth/2), body->position.y+(screenHeight/2), body->radius, color);
  }
}


int main(int argc, char *argv[]){
  //Setting SDL2 up
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Window* window = SDL_CreateWindow("Collision Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
  bool running = true;
  SDL_Event event;
  
  //Setting Simulation up
  SDL_Color colorWhite;
  colorWhite.r = 255;
  colorWhite.g = 255;
  colorWhite.b = 255;
  colorWhite.a = 255;

  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;
  
  simulation runningSimulation(3000,1000);
  runningSimulation.addBody(vector2(-50,10), 1, 10, vector2(5,0));
  runningSimulation.addBody(vector2(100,10), 1, 10, vector2(-5,0));
  
  fmt::print("Simulation Starting \n");

  //Event loop
  while (running){
    
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    deltaTime = ((double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency()))*0.001;
    
    if (SDL_PollEvent(&event)) {
      /* an event was found */
      switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
          running = false;
          break;
      }
    }
    
    runningSimulation.handleTick(deltaTime);

    DrawBodies(renderer, runningSimulation.simulationBodies,colorWhite);
    /* update the screen */
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
  }

  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
