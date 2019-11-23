/*
 * BasicTest.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "railroad/railroad.h"

using ::railroad::binds;  // aka bindSuccess

int main(int /* argc */, char** /* argv */) {
  std::cout << "Start Testing" << std::endl;

  std::function<int(int)> adder = [](int i) { return i + 1; };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<int>;

  for (int i = -5; i <= 5; ++i) {
    int normalResult = adder(i);
    std::optional<int> bindResult = (feed >> binds(adder) >> terminate)(i);
    if (!bindResult) {
      return 1;
    }
    if (normalResult != *(bindResult)) {
      return 2;
    }
  }

  std::cout << "Success" << std::endl;
  return 0;
}
