#include "Bullet.h"
#include "PCharacter.h"

#ifndef __OOP_H__
#define __OOP_H__

float checkBorders(float, float);
int moveCharacter(sf::Time, float, float);
bool checkCollision(Bullet&, PCharacter);
struct base_values;
base_values getBaseVals(int);
std::vector<int> getShift(int64_t);
std::vector<int> updateCoordinates_parabolic(Bullet&, sf::Clock&);
std::string fixedFloat(float);
#endif