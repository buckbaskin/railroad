/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "railroad/railroad.h"

using ::railroad::rbinds;  // aka bindSuccess

int main(int /* argc */, char** /* argv */) {
  std::function<int(int)> adder = [](int i) { return i + 1; };
  std::function<std::string(int)> stringify = [](int i) {
    std::stringstream buf;
    buf << "You can do anything " << i << " time(s)";
    return buf.str();
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<std::string>;

  // clang-format off
  std::cout << "\">> Composed\" Functions:" << std::endl <<
           *((feed
              >> rbinds(adder)
              >> rbinds(adder)
              >> rbinds<int, int>([](int i) { return i - 1; })
              >> rbinds(stringify) >>
              terminate)(41))
            << std::endl;
  // clang-format on
  return 0;
}
