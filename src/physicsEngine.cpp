#include <iostream>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>

using degrees = int;
using radians = float;
#define PI 3.14159

float degreesToRadians(degrees angle){
  return angle*(PI/180);
}

float radiansToDegrees(radians angle){
  return angle * (180/PI);
}

float precisionRound(float var)
{
  // 37.66666 * 100 =3766.66
  // 3766.66 + .5 =3767.16    for rounding off value
  // then type cast to int so value is 3767
  // then divided by 100 so the value converted into 37.67
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

struct vector2 {
  float x;
  float y;
  friend vector2  operator+(const vector2 vector);
  friend std::ostream& operator<<(std::ostream& os, const vector2& vector);
  vector2();
  vector2(float inputX, float inputY);
};

vector2 operator+(const vector2 inputVector1, const vector2 inputVector2){
  vector2 returnVector(0,0);
  returnVector.x = inputVector1.x + inputVector2.x;
  returnVector.y = inputVector1.y + inputVector2.y;
  return returnVector;
}

std::ostream& operator<<(std::ostream& os, const vector2& vector){
  os << "(" << vector.x << "," << vector.y << ")";
  return os;
}

vector2::vector2() : x(0), y(0) {}

vector2::vector2(float inputX, float inputY) : x(inputX), y(inputY) {}

struct force {
  degrees direction;
  float magnitude;
  force(degrees inputDirection, float inputMagnitude);
};

force::force(degrees inputDirection, float inputMagnitude) : direction(inputDirection), magnitude(inputMagnitude) {}


vector2 forceToVector(const force inputForce){
  vector2 returnVector;
  returnVector.x = inputForce.magnitude * cos(degreesToRadians(inputForce.direction));
  returnVector.y = inputForce.magnitude * sin(degreesToRadians(inputForce.direction));
  return returnVector;
}

force vectorToForce(const vector2 vector){
  force returnForce(0,0);
  returnForce.magnitude = sqrt(pow(vector.x,2) + pow(vector.y,2));
  returnForce.direction = round(radiansToDegrees(atan2(vector.y,vector.x)));
  return returnForce;
}

struct body {
  vector2 position;
  float mass = 1;
  float radius = 1;
  vector2 speed;
  vector2 speedToAdd;
  body(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed = vector2(), vector2 inputSpeedToAdd = vector2());
};

body::body(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed, vector2 inputSpeedToAdd) : position(inputPosition), mass(inputMass), radius(inputRadius), speed(inputSpeed), speedToAdd(inputSpeedToAdd) {}

std::pair<float,float> solve1DCollision(float b1SpeedComponent, float b2SpeedComponent, float b1Mass, float b2Mass){
  std::pair<float,float> returnPair;
  returnPair.first = ((b1SpeedComponent * (b1Mass - b2Mass)) + (2 * (b2Mass * b2SpeedComponent)))/(b1Mass + b2Mass);
  returnPair.second = ((b2SpeedComponent * (b2Mass - b1Mass)) + (2 * (b1Mass * b1SpeedComponent)))/(b1Mass + b2Mass);
  return returnPair;
} 
