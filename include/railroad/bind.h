/*
 * bind.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/PartialFailureResult.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/Result.h"
#include "railroad/bindf.h"
#include "railroad/binds.h"

namespace railroad {

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<
     Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
         nakedFunc) {
  return nakedFunc;
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<
     PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
         nakedFunc) {
  return binds<OutputType, InputType, OutputFailureType, InputFailureType>(nakedFunc);
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bindr(std::function<PartialFailureResult<OutputFailureType>(
         PartialFailureResult<InputFailureType>)>
         nakedFunc) {
  return bindf<OutputType, InputType, OutputFailureType, InputFailureType>(nakedFunc);
}

}  // namespace railroad