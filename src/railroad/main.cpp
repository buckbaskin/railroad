/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include <string>

#include "railroad/Result.h"
#include "railroad/abc.h"
#include "railroad/bind.h"

class Increment : public ::railroad::abc::Callable1<int, int> {
 public:
  int operator()(const int& input) const override { return input + 1; }
};

std::ostream& operator<<(std::ostream& out, const Increment& /* inc */) {
  out << "Increment__function(int) -> int + 1";
  return out;
}

int main(int /* argc */, char** /* argv */) {
  using ::railroad::Result;
  using InputResult = Result<int, std::string>;
  using OutputResult = Result<int, std::string>;

  int rawInput = 0;

  Increment successOnlyFunction;
  int directOutput = successOnlyFunction(successOnlyFunction(rawInput));

  InputResult input = InputResult::Success(rawInput);
  ::railroad::bind::SuccessPipe railroadIncrementFunction{successOnlyFunction};
  OutputResult output =
      railroadIncrementFunction(railroadIncrementFunction(input));

  int railroadOutput = output.getSuccess().unpack();

  if (directOutput == railroadOutput) {
    std::cout << "Successfully bound " << successOnlyFunction << " to get "
              << railroadOutput << " from " << rawInput << std::endl;
  } else {
    std::cout << "Binding failed for " << successOnlyFunction << " to get "
              << railroadOutput << " from " << rawInput << std::endl;
  }

  return 0;
}
