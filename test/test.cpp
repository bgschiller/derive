#include "showy-meal.h"
#include <iostream>
#include <string>

Breakfast bfast{4, std::string("tea")};
int main() { std::cout << bfast.toString() << std::endl; }
