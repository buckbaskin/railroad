/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "railroad/binds.h"
#include "railroad/feed.h"
#include "railroad/style.h"
#include "railroad/terminate.h"

using ::railroad::binds;
using ::railroad::Result;

int main(int /* argc */, char** /* argv */) {
  std::function<int(int)> adder = [](int i) { return i + 1; };
  std::function<std::string(int)> stringify = [](int i) {
    std::stringstream buf;
    buf << i << " ;) ";
    return buf.str();
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::unsafe::terminateSuccess<std::string>;

  // clang-format off
  std::cout << "Maybe? "
            << (feed
              >> binds(adder)
              >> binds(adder)
              >> binds<int, int>([](int i) { return i - 1; })
              >> binds(stringify) >>
              terminate)(0)
            << " . Did it work?" << std::endl;
  // clang-format on
  return 0;
}
