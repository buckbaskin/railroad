/*
 * binds.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/Result.h"

namespace railroad {

template <typename OutputType, typename InputType,
          typename OutputFailureType = ::railroad::DefaultFailure,
          typename InputFailureType = ::railroad::DefaultFailure>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
binds(std::function<OutputType(InputType)> nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    } else {
      return Success<OutputType, OutputFailureType>(
          nakedFunc(input.getSuccess()));
    }
  };
}

template <typename OutputType, typename InputType,
          typename OutputFailureType = ::railroad::DefaultFailure,
          typename InputFailureType = ::railroad::DefaultFailure>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
binds(std::function<
      PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    } else {
      return Success<OutputType, OutputFailureType>(
          nakedFunc(input.getSuccessPartial()));
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
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    } else {
      return Success<OutputType, OutputFailureType>(
          nakedFunc(input.getSuccess()));
    }
  };
}

}  // namespace railroad
