#include <iostream>
#include <sdl.h>
#include <fmt/core.h>
#include <physicsEngine.hpp>
#include <utility>

using namespace std;

int main(int argc, char *argv[]){
  body b1(vector2(0,0), 1, 1, vector2(1,0));
  body b2(vector2(2,0), 2, 1, vector2(0,0.50));
  std::pair<vector2,vector2> result = solve2DCollision(&b1,&b2);
  
  fmt::print("({},",result.first.x);
  fmt::print("{})\n",result.first.y);

  fmt::print("({},",result.second.x);
  fmt::print("{})\n",result.second.y);
	return 0;
}
