#include <iostream>
#include <sdl.h>
#include <fmt/core.h>
#include <physicsEngine.hpp>

using namespace std;

int main(int argc, char *argv[]){
  vector2 v = forceToVector(force(53,5));
  fmt::print("({},{}) \n", v.x,v.y);
  force f1 = vectorToForce(vector2(3,4));
  fmt::print("Magnitude is {} \n",f1.magnitude);
  fmt::print("Direction is: {} \n",f1.direction);
	return 0;
}
