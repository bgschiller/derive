#include "derive.h"
#include <string>

struct Breakfast : Derive::show {
  int numPancakes;
  std::string beverage;
};
