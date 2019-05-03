#include "gamelib/Headers/Resources.h"

Resources::Resources(
  int _water, int _food, int _wood, int _metal, int _fuel, int _med) :
  water(_water),
  food(_food), wood(_wood), metal(_metal), fuel(_fuel), med(_med)
{
}

Resources Resources::takeDistribution(int amount)
{
  Resources return_resource;
  while (amount > 0 && total() > 0)
  {
    return_resource.wood += takeIfAvailable(wood, amount);
    return_resource.metal += takeIfAvailable(metal, amount);
    return_resource.med += takeIfAvailable(med, amount);
    return_resource.food += takeIfAvailable(food, amount);
    return_resource.fuel += takeIfAvailable(fuel, amount);
    return_resource.water += takeIfAvailable(water, amount);
  }
  return return_resource;
}

int Resources::takeIfAvailable(int& resource, int& amount)
{
  if (resource > 0 && amount > 0)
  {
    resource--;
    amount--;
    return 1;
  }
  return 0;
}

Resources Resources::operator+(Resources& resources)
{
  this->wood += resources.wood;
  this->metal += resources.metal;
  this->med += resources.med;
  this->food += resources.food;
  this->fuel += resources.fuel;
  this->water += resources.water;
  return *this;
}

Resources Resources::operator-(Resources& resources)
{
  this->wood -= resources.wood;
  this->metal -= resources.metal;
  this->med -= resources.med;
  this->food -= resources.food;
  this->fuel -= resources.fuel;
  this->water -= resources.water;
  return *this;
}

bool Resources::canCombineWith(const Resources& resources)
{
  return (med + resources.med) >= 0 && (wood + resources.wood) >= 0 &&
         (metal + resources.metal) >= 0 && (food + resources.food) >= 0 &&
         (water + resources.water) >= 0 && (fuel + resources.fuel) >= 0;
}