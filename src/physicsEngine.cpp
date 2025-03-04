#include <iostream>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>
#include <execution>
#include <memory>

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

  friend bool  operator==(const vector2 inputVector1, const vector2 inputVector2);
  friend vector2  operator+(const vector2 inputVector1, const vector2 inputVector2);
  friend vector2 operator-(const vector2 inputVector1, const vector2 inputVector2);
  friend std::ostream& operator<<(std::ostream& os, const vector2& vector);

  vector2();
  vector2(float inputX, float inputY);
} vector2;

bool operator==(const vector2 inputVector1, const vector2 inputVector2){
  if (inputVector1.x == inputVector2.x && inputVector1.y == inputVector2.y){
    return true;
  }
  return false;
}

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

struct vector2HashFunction 
{ 
  size_t operator()(const vector2 &v) const
  { 
    return pow(v.x,v.y); 
  } 
}; 

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
  ~body();
};

body::body(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed, vector2 inputSpeedToAdd) : position(inputPosition), mass(inputMass), radius(inputRadius), speed(inputSpeed), speedToAdd(inputSpeedToAdd) {}

body::~body(){}


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



typedef struct spatialGrid {
  std::unique_ptr<std::unordered_map<vector2, std::unordered_set<body*>, vector2HashFunction>> cells = std::make_unique<std::unordered_map<vector2, std::unordered_set<body*>, vector2HashFunction>>();

  float cellWidth = 1;
  float cellHeight = 1;

  float minimumCellXinHash;
  float maximumCellXinHash;
  
  float minimumCellYinHash;
  float maximumCellYinHash;
  
  vector2 getCellOfCoordinates(float x, float y);
  void insertBodyIntoCells(body* b1);
  void clearCells();
  void setup(float iCellWidth, float iCellHeight, float simulationWidth, float simulationHeight);


  spatialGrid();

} spatialGrid;

vector2 spatialGrid::getCellOfCoordinates(float x, float y){
  const float rX = std::floor(x/cellWidth);
  const float rY = std::floor(y/cellHeight);

  return vector2(rX, rY);
}

void spatialGrid::insertBodyIntoCells(body* b1){
  const float minX = b1->position.x - b1->radius;
  const float maxX = b1->position.x + b1->radius;

  const float minY = b1->position.y - b1->radius;
  const float maxY = b1->position.y + b1->radius;

  vector2 debugInfo = getCellOfCoordinates(minX, minY);
  cells->find(getCellOfCoordinates(minX, minY))->second.insert(b1);
  debugInfo = getCellOfCoordinates(minX, minY);
  cells->find(getCellOfCoordinates(maxX, minY))->second.insert(b1);
  cells->find(getCellOfCoordinates(minX, maxY))->second.insert(b1);
  cells->find(getCellOfCoordinates(maxX, maxY))->second.insert(b1);

  return;
}

void spatialGrid::clearCells(){
  for (auto i : (*cells)){
    i.second.clear();
  }
}

void spatialGrid::setup(float iCellWidth, float iCellHeight, float simulationWidth, float simulationHeight){

  cellHeight = iCellHeight;
  cellWidth = iCellWidth;

  const int numberOfCellsPerColum = std::ceil(simulationHeight/iCellHeight);
  const int numberOfCellsPerRow = std::ceil(simulationWidth/iCellWidth);

  minimumCellXinHash = 0 - std::ceil(numberOfCellsPerRow/2) - 1;
  maximumCellXinHash = std::floor(numberOfCellsPerRow/2); 

  minimumCellYinHash = 0 - std::ceil(numberOfCellsPerColum/2) - 1;
  maximumCellYinHash = std::floor(numberOfCellsPerColum/2);

  //Distributes the number of cells per row uniformily below and above 0
  for (int x = minimumCellXinHash; x <= maximumCellXinHash; ++x){
    //Distributes the number of cells per colum uniformily below and above 0
    for (int y = minimumCellYinHash; y <= maximumCellYinHash; ++y){
      (*cells)[vector2(x,y)] = std::unordered_set<body*>{};  
    }
  }

}

spatialGrid::spatialGrid(){}

typedef struct simulation{
  std::vector<std::unique_ptr<body>> simulationBodies;
  float simulationWidth;
  float simulationHeight;
  spatialGrid collisionGrid;
  void handleTick(double dt);
  void addBody(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed = vector2(), vector2 inputSpeedToAdd = vector2());
  
  simulation(float iSimulationWidth, float iSimulationHeight);
} simulation;

simulation::simulation(float iSimulationWidth, float iSimulationHeight){
  simulationWidth = iSimulationWidth;
  simulationHeight = iSimulationHeight;
  collisionGrid.setup(60, 500, iSimulationWidth, iSimulationHeight);
}

void simulation::addBody(vector2 inputPosition, float inputMass, float inputRadius, vector2 inputSpeed, vector2 inputSpeedToAdd){
  std::unique_ptr<body> b = std::make_unique<body>(inputPosition, inputMass, inputRadius, inputSpeed, inputSpeedToAdd);
  simulationBodies.push_back(std::move(b));
}

void simulation::handleTick(double dt){
  
  for (auto& body : simulationBodies){
    collisionGrid.insertBodyIntoCells(body.get());
  }

  for (auto cell : (*collisionGrid.cells)){

    if (cell.second.size() > 1){
      std::vector<std::pair<body*,body*>> searched;

      for (auto body : cell.second){

        for (auto secondBody : cell.second){

          bool isSecondBodyAlreadySearched = false;
          for (auto pair : searched){
            if (pair.first == secondBody && pair.second == body){
              isSecondBodyAlreadySearched = true;
              break;
            }
          }
          if (secondBody != body && !isSecondBodyAlreadySearched){
            searched.push_back({body, secondBody});
            if (isColliding(body, secondBody)){

              std::cout << "Old Speed: " << "(" << body->speed.x << "," << body->speed.y << ") and (" << secondBody->speed.x << "," << secondBody->speed.y << ") \n";
              const std::pair<vector2, vector2> newVelocities = solve2DCollision(body, secondBody);
              std::cout << "New Speed: " << newVelocities.first << ", " << newVelocities.second << "\n";

              printf("Break \n");

              body->speed = newVelocities.first;
              secondBody->speed = newVelocities.second;
            }
          }
        }
      }
    }
  }
  std::for_each(std::execution::par_unseq,simulationBodies.begin(), simulationBodies.end(), [=](std::unique_ptr<body>& b){
    vector2 timedVector = vector2(b->speed.x * dt, b->speed.y * dt);
    b->position = b->position + timedVector;
  });

  collisionGrid.clearCells();
}
