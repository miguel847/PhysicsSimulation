#pragma once

#include <iostream>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>

using degrees = int;
using radians = float;
#define PI 3.14159

float degreesToRadians(degrees angle);
float radiansToDegrees(radians angle);
float precisionRound(float var);

typedef struct vector2 {
  float x;
  float y;

  vector2();
  vector2(float inputX, float inputY);

  friend bool  operator==(const vector2 inputVector1, const vector2 inputVector2);
  friend vector2 operator+(const vector2 inputVector1, const vector2 inputVector2); 
  friend vector2 operator-(const vector2 inputVector1, const vector2 inputVector2);
  friend std::ostream& operator<<(std::ostream& os, const vector2& vector);
} vector2;

struct vector2HashFunction 
{ 
  size_t operator()(const vector2 &v) const;
}; 


float dotProduct(const vector2 inputVector1, const vector2 inputVector2);
vector2 multiplyVectorByScalar(const vector2 inputVector1, const float scalar);

struct force {
  degrees direction;
  float magnitude;
  force(degrees inputDirection, float inputMagnitude);
};

struct body {
  vector2 position;
  float mass = 1;
  float radius = 1;
  vector2 speed;
  vector2 speedToAdd;
  body(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed = vector2(), vector2 inputSpeedToAdd = vector2());
};

bool isColliding(body* b1, body* b2);


vector2 forceToVector(const force inputForce);
force vectorToForce(const vector2 vector);

std::pair<float,float> solve1DCollision(float b1SpeedComponent, float b2SpeedComponent, float b1Mass, float b2Mass); 
std::pair<vector2, vector2> solve2DCollision(body* b1, body* b2);


typedef struct spatialGrid {
  std::unordered_map<vector2, std::vector<body*>, vector2HashFunction> cells;

  float cellWidth = 1;
  float cellHeight = 1;

  void insertBodyIntoCells(body* b1);
  void setup(float iCellWidth, float iCellHeight, float simulationWidth, float simulationHeight);

  spatialGrid();

} spatialGrid;

typedef struct simulation{
  std::vector<body*> simulationBodies;
  float simulationWidth;
  float simulationHeight;
  spatialGrid collisionGrid;
  void handleTick(double dt);
  void addBody(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed = vector2(), vector2 inputSpeedToAdd = vector2());

  simulation(float iSimulationWidth, float iSimulationHeight);

} simulation;

