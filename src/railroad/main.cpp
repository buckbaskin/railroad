/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <iostream>
#include <sstream>
#include <string>

#include "railroad/FailurePipe.h"
#include "railroad/Result.h"
#include "railroad/SuccessPipe.h"
#include "railroad/ValidateSuccess.h"
#include "railroad/abc.h"
#include "railroad/terminate.h"

class Increment : public ::railroad::abc::Callable1<int, int> {
 public:
  int operator()(const int& input) const override { return input + 1; }
};

std::ostream& operator<<(std::ostream& out, const Increment& /* inc */) {
  out << "Increment__function(int) -> int + 1";
  return out;
}

class StringWriter
    : public ::railroad::abc::Callable1<std::string, std::string> {
 public:
  std::string operator()(const std::string& input) const override {
    if (input.size() > 0) {
      return "Hello World";
    }
    return input;
  }
};

class RangeCheck
    : public ::railroad::abc::Callable1<::railroad::Result<int, std::string>,
                                        int> {
  using OutputResult = ::railroad::Result<int, std::string>;

 public:
  OutputResult operator()(const int& input) const override {
    if (0 <= input && input <= 1) {
      return OutputResult::Success(input);
    } else {
      std::stringstream buf;
      buf << "Integer Input Out of Range: " << input << " not in [0, 1]";
      return OutputResult::Failure(buf.str());
    }
  }
};

int main(int /* argc */, char** /* argv */) {
  using ::railroad::Result;
  using InputResult = Result<int, std::string>;
  using OutputResult = Result<int, std::string>;

  int rawInput = 0;
  std::string strInput = "";

  Increment successOnlyFunction;
  int directOutput = successOnlyFunction(successOnlyFunction(rawInput));

  StringWriter failureOnlyFunction;
  std::string directFailText = failureOnlyFunction(strInput);

  RangeCheck validateAndSplit;
  int inRange = validateAndSplit(0).getSuccess().unpack();
  std::string outOfRange = validateAndSplit(2).getFailure().unpack();

  InputResult input = InputResult(rawInput, strInput);

  ::railroad::bind::SuccessPipe railroadIncrementFunction{successOnlyFunction};
  ::railroad::bind::FailurePipe<int, int, std::string, std::string>
      railroadStringFunction{failureOnlyFunction};
  ::railroad::bind::ValidateSuccess railroadFromSplit(validateAndSplit);

  OutputResult output = railroadFromSplit(railroadStringFunction(
      railroadIncrementFunction(railroadIncrementFunction(input))));

  ::railroad::terminate::with::SuccessOnly<int, std::string> terminate;
  std::optional<int> possibleSuccess = terminate(output);
  if (!possibleSuccess) {
    std::cout << "Optional Contains no success termination" << std::endl;
  } else {
    std::cout << "Optional Contains " << *possibleSuccess << std::endl;
  }

  if (output.hasSuccess()) {
    int railroadOutput = output.getSuccess().unpack();

    if (directOutput == railroadOutput) {
      std::cout << "Successfully bound " << successOnlyFunction << " to get "
                << railroadOutput << " from " << rawInput << std::endl;
    } else {
      std::cout << "Binding failed for " << successOnlyFunction << " to get "
                << railroadOutput << " from " << rawInput << std::endl;
    }
  } else {
    std::cerr << "Output did not calculate happy path." << std::endl;
  }

  if (output.hasFailure()) {
    std::string railroadFailText = output.getFailure().unpack();
    std::cout << "Direct fail text: >" << outOfRange << "< vs railroad: >"
              << railroadFailText << "<" << std::endl;
  } else {
    std::cout << "No Failure text to report" << std::endl;
  }

  return 0;
}
