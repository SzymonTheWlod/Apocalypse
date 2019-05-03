#ifndef PROJECT_RESOURCES_H
#define PROJECT_RESOURCES_H

#include "NetworkPacket.h"

struct Resources
{
 public:
  Resources() = default;
  Resources(int _water, int _food, int _wood, int _metal, int _fuel, int _med);

  int water = 0;
  int food = 0;
  int wood = 0;
  int metal = 0;
  int fuel = 0;
  int med = 0;

  Resources operator-(Resources& resources);
  Resources operator+(Resources& resources);

  bool operator>=(Resources& resources)
  {
    return this->wood >= resources.wood && this->metal >= resources.metal &&
           this->med >= resources.med && this->food >= resources.food &&
           this->fuel >= resources.fuel && this->water >= resources.water;
  }

  int total() { return water + food + wood + metal + fuel + med; }
  Resources takeDistribution(int amount);
  bool canCombineWith(const Resources& resources);

 private:
  int takeIfAvailable(int& resource, int& amount);
};

#endif // PROJECT_RESOURCES_H
