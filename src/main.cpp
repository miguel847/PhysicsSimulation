#include <iostream>
#include <sdl.h>
#include <fmt/core.h>
#include <physicsEngine.hpp>

using namespace std;

int main(int argc, char *argv[]){
  fmt::print("{} \n",solve1DCollision(3,0,1,1).first);
	return 0;
}
