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

using ::railroad::binds;  // aka bindSuccess

int main(int /* argc */, char** /* argv */) {
  std::function<int(int)> adder = [](int i) { return i + 1; };
  std::function<std::string(int)> stringify = [](int i) {
    std::stringstream buf;
    buf << i << " ;) ";
    return buf.str();
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<std::string>;

  // clang-format off
  std::cout << "Maybe? "
            << *((feed
              >> binds(adder)
              >> binds(adder)
              >> binds<int, int>([](int i) { return i - 1; })
              >> binds(stringify) >>
              terminate)(0))
            << " . Did it work?" << std::endl;
  // clang-format on
  return 0;
}
