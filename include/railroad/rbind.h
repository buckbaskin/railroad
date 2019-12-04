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

// TODO: exception support for binds. Consider flipping binds/bind dependency
template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return rbinds<OutputType, InputType, OutputFailureType, InputFailureType>(
      nakedFunc);
}

// TODO: exception support for bindf. Consider flipping bindf/bind dependency
template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<PartialFailureResult<OutputFailureType>(
          PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return rbindf<OutputType, InputType, OutputFailureType, InputFailureType>(
      nakedFunc);
}

template <typename InputType, typename InputFailureType,
          typename ExceptionType = InputFailureType>
std::function<
    Result<InputType, InputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      PartialSuccessResult<InputType>(PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      try {
        return Success<InputType, InputFailureType>(
            nakedFunc(input.getFailurePartial()));
      } catch (const ExceptionType& e) {
        return Failure<InputType, InputFailureType>(e);
      }
    } else {
      return Success<InputType, InputFailureType>(input.getSuccessPartial());
    }
  };
}

template <typename InputType, typename InputFailureType,
          typename ExceptionType = InputFailureType>
std::function<
    Result<InputType, InputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      PartialFailureResult<InputFailureType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<InputType, InputFailureType>(input.getFailurePartial());
    } else {
      try {
        return Failure<InputType, InputFailureType>(
            nakedFunc(input.getSuccessPartial()));
      } catch (const ExceptionType& e) {
        return Failure<InputType, InputFailureType>(e);
      }
    }
  };
}

// 1 : 2

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      Result<OutputType, OutputFailureType>(PartialSuccessResult<InputType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      return Failure<OutputType, OutputFailureType>(input.getFailurePartial());
    } else {
      try {
        return nakedFunc(input.getSuccessPartial());
      } catch (const ExceptionType& e) {
        return Failure<OutputType, OutputFailureType>(e);
      }
    }
  };
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<Result<OutputType, OutputFailureType>(
          PartialFailureResult<InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    if (input.hasFailure()) {
      try {
        return nakedFunc(input.getFailurePartial());
      } catch (const ExceptionType& e) {
        return Failure<OutputType, OutputFailureType>(e);
      }
    } else {
      return Success<OutputType, OutputFailureType>(input.getSuccessPartial());
    }
  };
}

// 2:1

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<
      PartialSuccessResult<OutputType>(Result<InputType, InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    try {
      return Success<OutputType, OutputFailureType>(nakedFunc(input));
    } catch (const ExceptionType& e) {
      return Failure<OutputType, OutputFailureType>(e);
    }
  };
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType, typename ExceptionType = OutputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
rbind(std::function<PartialFailureResult<OutputFailureType>(
          Result<InputType, InputFailureType>)>
          nakedFunc) {
  return [nakedFunc](Result<InputType, InputFailureType> input) {
    try {
      return Failure<OutputType, OutputFailureType>(nakedFunc(input));
    } catch (const ExceptionType& e) {
      return Failure<OutputType, OutputFailureType>(e);
    }
  };
}

}  // namespace railroad
