/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

template <typename OutputType, typename HiddenType, typename InputType>
std::function<OutputType(InputType)> operator>>(
    std::function<HiddenType(InputType)> inner,
    std::function<OutputType(HiddenType)> outer) {
  return [inner, outer](InputType input) { return outer(inner(input)); };
}

int main(int /* argc */, char** /* argv */) {
  std::function<int(int)> adder = [](int i) { return i + 1; };
  std::function<int(int)> subtractor = [](int i) { return i - 1; };
  std::function<std::string(int)> stringify = [](int i) {
    std::stringstream buf;
    buf << i << ";)";
    return buf.str();
  };
  std::cout << "Maybe? " << (adder >> adder >> subtractor)(0)
            << " . Did it work?" << std::endl;
  return 0;
}
