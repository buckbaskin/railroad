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
#include "railroad/is_instantiation.h"

namespace railroad {

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindf(std::function<PartialFailureResult<OutputFailureType>(
          PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(
          nakedFunc(input.getFailurePartial()));
    } else {
      return Success<OutputType, OutputFailureType>(input.getSuccessPartial());
    }
  };
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindf(std::function<OutputFailureType(InputFailureType)> nakedFunc) {
  static_assert(
      !(is_instantiation<PartialFailureResult, InputFailureType>::value),
      "Don't use this bindf for known half functions");
  static_assert(
      !(is_instantiation<PartialFailureResult, OutputFailureType>::value),
      "Don't use this bindf for known half functions");
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(
          nakedFunc(input.getFailure()));
    } else {
      return Success<OutputType, OutputFailureType>(input.getSuccessPartial());
    }
  };
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename WrappedFunc>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindf(WrappedFunc nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    static_assert(
        !(is_instantiation<PartialFailureResult, InputFailureType>::value),
        "Don't use this bindf for known half functions");
    static_assert(
        !(is_instantiation<PartialFailureResult, OutputFailureType>::value),
        "Don't use this bindf for known half functions");
    static_assert(
        !(std::is_same<WrappedFunc,
                       std::function<PartialFailureResult<OutputFailureType>(
                           PartialFailureResult<InputFailureType>)>>::value),
        "Shouldn't bindf std::function<PFR<T>(PFR<T>)> to WrappedFunc");
    static_assert(!(std::is_same<WrappedFunc, std::function<OutputFailureType(
                                                  InputFailureType)>>::value),
                  "Shouldn't bindf std::function<T(T)> to WrappedFunc");

    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(
          nakedFunc(input.getFailure()));
    } else {
      return Success<OutputType, OutputFailureType>(input.getSuccessPartial());
    }
  };
}

}  // namespace railroad
