/*
 * main.cpp
 *
 * Copyright 2019 Buck Baskin
 */

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/Result.h"

using ::railroad::Result;

template <typename OutputType, typename InputType,
          typename OutputFailureType = ::railroad::DefaultFailure,
          typename InputFailureType = ::railroad::DefaultFailure>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
binds(std::function<OutputType(InputType)> nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasSuccess()) {
      return Result<OutputType, OutputFailureType>::Success(
          nakedFunc(input.getSuccess()), input.getFailurePartial());
    } else {
      return Result<OutputType, OutputFailureType>::Failure(
          input.getFailurePartial());
    }
  };
}

template <typename OutputType, typename InputType,
          typename OutputFailureType = ::railroad::DefaultFailure,
          typename InputFailureType = ::railroad::DefaultFailure,
          typename WrappedFunc>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
binds(WrappedFunc nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasSuccess()) {
      return Result<OutputType, OutputFailureType>::Success(
          nakedFunc(input.getSuccess()), input.getFailurePartial());
    } else {
      return Result<OutputType, OutputFailureType>::Failure(
          input.getFailurePartial());
    }
  };
}

template <typename InputType,
          typename InputFailureType = ::railroad::DefaultFailure>
Result<InputType, InputFailureType> feedSuccess(InputType input) {
  return Result<InputType, InputFailureType>::Success(input);
}

template <typename InputType = ::railroad::DefaultSuccess,
          typename InputFailureType>
Result<InputType, InputFailureType> feedFailure(InputFailureType input) {
  return Result<InputType, InputFailureType>::Failure(input);
}

template <typename OutputType,
          typename OutputFailureType = ::railroad::DefaultFailure>
OutputType terminateUnsafe(Result<OutputType, OutputFailureType> input) {
  return input.getSuccess();
}

template <typename OutputType = ::railroad::DefaultSuccess,
          typename OutputFailureType>
OutputFailureType terminateUnsafeFailure(
    Result<OutputType, OutputFailureType> input) {
  return input.getFailure();
}

template <typename OutputType, typename HiddenType, typename InputType>
std::function<OutputType(InputType)> operator>>(
    std::function<HiddenType(InputType)> inner,
    std::function<OutputType(HiddenType)> outer) {
  return [inner, outer](InputType input) { return outer(inner(input)); };
}

int main(int /* argc */, char** /* argv */) {
  std::function<int(int)> adder = [](int i) { return i + 1; };
  // std::function<int(int)> subtractor = [](int i) { return i - 1; };
  std::function<std::string(int)> stringify = [](int i) {
    std::stringstream buf;
    buf << i << " ;) ";
    return buf.str();
  };

  std::function feed = feedSuccess<int>;
  std::function terminate = terminateUnsafe<std::string>;

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
