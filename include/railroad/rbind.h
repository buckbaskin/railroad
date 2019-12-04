/*
 * rbind.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>
#include <type_traits>

#include "railroad/PartialFailureResult.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/Result.h"
#include "railroad/rbindf.h"
#include "railroad/rbinds.h"

namespace railroad {

// 2:2

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<Result<OutputType, OutputFailureType>(
          Result<InputType, InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    try {
      return nakedFunc(input);
    } catch (const ExceptionType& e) {
      return Failure<OutputType, OutputFailureType>(e);
    }
  };
}

// 1:1

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return rbinds<OutputType, InputType, OutputFailureType, InputFailureType>(
      nakedFunc);
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<PartialFailureResult<OutputFailureType>(
          PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return rbindf<OutputType, InputType, OutputFailureType, InputFailureType>(
      nakedFunc);
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
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

template <typename InputType, typename InputFailureType>
std::function<
    Result<InputType, InputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      PartialFailureResult<InputFailureType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  static_assert(std::is_same<InputType, InputType>::value,
                "bind SF won't convert output success types");
  static_assert(std::is_same<InputFailureType, InputFailureType>::value,
                "bind SF won't convert output failure types");
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<InputType, InputFailureType>(input.getFailurePartial());
    } else {
      return Failure<InputType, InputFailureType>(
          nakedFunc(input.getSuccessPartial()));
    }
  };
}

// 1 : 2

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
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
rbind(std::function<Result<OutputType, OutputFailureType>(
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
rbind(std::function<
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
rbind(std::function<PartialFailureResult<OutputFailureType>(
          Result<InputType, InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    return Failure<OutputType, OutputFailureType>(nakedFunc(input));
  };
}

}  // namespace railroad
