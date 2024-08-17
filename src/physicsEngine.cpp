#include <iostream>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <execution>

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

typedef struct vector2 {
  float x;
  float y;
  friend vector2  operator+(const vector2 inputVector1, const vector2 inputVector2);
  friend vector2 operator-(const vector2 inputVector1, const vector2 inputVector2);
  friend std::ostream& operator<<(std::ostream& os, const vector2& vector);
  vector2();
  vector2(float inputX, float inputY);
} vector2;

vector2 operator+(const vector2 inputVector1, const vector2 inputVector2){
  vector2 returnVector(0,0);
  returnVector.x = inputVector1.x + inputVector2.x;
  returnVector.y = inputVector1.y + inputVector2.y;
  return returnVector;
}
vector2 operator-(const vector2 inputVector1, const vector2 inputVector2){
  vector2 returnVector(0,0);
  returnVector.x = inputVector1.x - inputVector2.x;
  returnVector.y = inputVector1.y - inputVector2.y;
  return returnVector;
}
std::ostream& operator<<(std::ostream& os, const vector2& vector){
  os << "(" << vector.x << "," << vector.y << ")";
  return os;
}
vector2::vector2() : x(0), y(0) {}
vector2::vector2(float inputX, float inputY) : x(inputX), y(inputY) {}

float dotProduct(const vector2 inputVector1, const vector2 inputVector2){
  return (inputVector1.x * inputVector2.x + inputVector1.y * inputVector2.y);
}

vector2 multiplyVectorByScalar(const vector2 inputVector1, const float scalar){
  vector2 returnVector = vector2(scalar*inputVector1.x,scalar*inputVector1.y);
  return returnVector;
}

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

bool isColliding(body* b1, body* b2){
  const float distanceX = b1->position.x - b2->position.x;
  const float distanceY = b1->position.y - b2->position.y;
  const vector2 vecDistance(distanceX, distanceY);
  const float magnitudeOfDistance = sqrt(pow(vecDistance.x,2) + pow(vecDistance.y,2));
  
  if (magnitudeOfDistance <= b1->radius + b2->radius){
    return true;
  }
  return false;

}

std::pair<float,float> solve1DCollision(float b1SpeedComponent, float b2SpeedComponent, float b1Mass, float b2Mass){
  std::pair<float,float> returnPair;
  returnPair.first = ((b1SpeedComponent * (b1Mass - b2Mass)) + (2 * (b2Mass * b2SpeedComponent)))/(b1Mass + b2Mass);
  returnPair.second = ((b2SpeedComponent * (b2Mass - b1Mass)) + (2 * (b1Mass * b1SpeedComponent)))/(b1Mass + b2Mass);
  return returnPair;
}

std::pair<vector2, vector2> solve2DCollision(body* b1, body* b2){
  vector2 normalVector = b2->position - b1->position;
  float magnitudeOfNormalVector = sqrt(pow(normalVector.x,2) + pow(normalVector.y,2));
  vector2 unitaryNormalVector = vector2(normalVector.x/magnitudeOfNormalVector, normalVector.y/magnitudeOfNormalVector);
  vector2 unitaryTangentVector = vector2(-unitaryNormalVector.y, -unitaryNormalVector.x);

  float v1n = dotProduct(b1->speed,unitaryNormalVector);
  float v1t = dotProduct(b1->speed,unitaryTangentVector);
  float v2n = dotProduct(b2->speed,unitaryNormalVector);
  float v2t = dotProduct(b2->speed,unitaryTangentVector);

  std::pair<float, float> newVelocities = solve1DCollision(v1n, v2n, b1->mass, b2->mass); 
  
  vector2 v1nf = multiplyVectorByScalar(unitaryNormalVector,newVelocities.first);
  vector2 v1tf = multiplyVectorByScalar(unitaryTangentVector, v1t);
  vector2 v2nf = multiplyVectorByScalar(unitaryNormalVector,newVelocities.second);
  vector2 v2tf = multiplyVectorByScalar(unitaryTangentVector, v2t);

  std::pair<vector2, vector2> returnVector;
  returnVector.first = v1nf + v1tf;
  returnVector.second = v2nf + v2tf;
  return returnVector;
}

typedef struct simulation{
  std::vector<body*> simulationBodies;
  void handleTick(double dt);
  void addBody(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed = vector2(), vector2 inputSpeedToAdd = vector2());
} simulation;

void simulation::addBody(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed, vector2 inputSpeedToAdd){
  body* b = new body(inputPosition, inputMass, inputRadius, inputSpeed, inputSpeedToAdd);
  simulationBodies.push_back(b);
}

void simulation::handleTick(double dt){

  std::for_each(std::execution::par_unseq,simulationBodies.begin(), simulationBodies.end(), [=](body* b){
    vector2 timedVector = vector2(b->speed.x * dt, b->speed.y * dt);
    b->position = b->position + timedVector;
  });
}
