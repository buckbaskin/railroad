/*
 * bind.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>
#include <type_traits>

#include "railroad/PartialFailureResult.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/Result.h"
#include "railroad/bindf.h"
#include "railroad/binds.h"

namespace railroad {

// 2:2

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<Result<OutputType, OutputFailureType>(
          Result<InputType, InputFailureType>)>
          nakedFunc) {
  return nakedFunc;
}

// 1:1

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<
      PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return binds<OutputType, InputType, OutputFailureType, InputFailureType>(
      nakedFunc);
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<PartialFailureResult<OutputFailureType>(
          PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return bindf<OutputType, InputType, OutputFailureType, InputFailureType>(
      nakedFunc);
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<
      PartialSuccessResult<OutputType>(PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Success<OutputType, OutputFailureType>(
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
bindr(std::function<
      PartialFailureResult<OutputFailureType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  static_assert(std::is_same<OutputFailureType, InputFailureType>::value,
                "bind SF won't convert output failure types");
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    } else {
      return Failure<OutputType, OutputFailureType>(
          nakedFunc(input.getSuccessPartial()));
    }
  };
}

// 1 : 2

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<
      Result<OutputType, OutputFailureType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    } else {
      return nakedFunc(input.getSuccessPartial());
    }
  };
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<Result<OutputType, OutputFailureType>(
          PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return nakedFunc(input.getFailurePartial());
    } else {
      return Success<OutputType, OutputFailureType>(input.getSuccessPartial());
    }
  };
}

// 2:1

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<
      PartialSuccessResult<OutputType>(Result<InputType, InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    return Success<OutputType, OutputFailureType>(nakedFunc(input));
  };
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<PartialFailureResult<OutputFailureType>(
          Result<InputType, InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    return Failure<OutputType, OutputFailureType>(nakedFunc(input));
  };
}

}  // namespace railroad