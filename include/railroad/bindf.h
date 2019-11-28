/*
 * bindf.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/PartialFailureResult.h"
#include "railroad/Result.h"

namespace railroad {

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
class Bindfc {
 public:
  std::function<Result<OutputType, OutputFailureType>(
      Result<InputType, InputFailureType>)>
  operator()(std::function<OutputFailureType(InputFailureType)> nakedFunc);

 private:
  std::function<Result<OutputType, OutputFailureType>(
      Result<InputType, InputFailureType>)>
  call(std::function<OutputFailureType(InputFailureType)> nakedFunc);
};

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
inline std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
Bindfc<OutputType, InputType, OutputFailureType, InputFailureType>::call(
    std::function<OutputFailureType(InputFailureType)> nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(
          nakedFunc(input.getFailure()));
    } else {
      return Success<OutputType, OutputFailureType>(input.getSuccessPartial());
    }
  };
}

/* Can't partial specializate this function

template <typename OutputFailureType, typename InputFailureType>
inline std::function<Result<DefaultSuccess, OutputFailureType>(
    Result<DefaultSuccess, InputFailureType>)>
Bindfc<DefaultSuccess, DefaultSuccess, OutputFailureType, InputFailureType>::
operator()(std::function<OutputFailureType(InputFailureType)> nakedFunc) {
  return call(nakedFunc);
}
*/

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>bindf(std::function<
      PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasSuccess()) {
      return Success<OutputType, OutputFailureType>(
          nakedFunc(input.getSuccessPartial()), input.getFailurePartial());
    } else {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    }
  };
}

template <typename OutputType, typename InputType,
          typename OutputFailureType = ::railroad::DefaultFailure,
          typename InputFailureType = ::railroad::DefaultFailure,
          typename WrappedFunc>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindf(WrappedFunc nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasSuccess()) {
      return Success<OutputType, OutputFailureType>(
          nakedFunc(input.getSuccess()), input.getFailurePartial());
    } else {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    }
  };
}

}  // namespace railroad
