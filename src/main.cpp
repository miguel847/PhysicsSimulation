#include <iostream>
#include <sdl.h>
#include <physicsEngine.hpp>

using namespace std;

int main(int argc, char *argv[]){
  cout << forceToVector(force(53,5)) << endl;
  force f1 = vectorToForce(vector2(3,4));
  cout << "Magnitude is: " << f1.magnitude << endl;
  cout << "Direction is: " << f1.direction << endl;
	return 0;
}
