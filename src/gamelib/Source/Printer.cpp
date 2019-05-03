#include "gamelib/Headers/Printer.h"
#include <iostream>

GameLib::Printer::~Printer()
{
  std::lock_guard<std::mutex> guard(cout_mtx);
  std::cout.clear();
  std::cout << this->str();
}

std::mutex GameLib::Printer::cout_mtx{};