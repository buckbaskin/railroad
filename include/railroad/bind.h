/*
 * bind.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/Result.h"
#include "railroad/binds.h"
#include "railroad/bindf.h"
#include "railroad/PartialSuccessResult.h"
#include "railroad/PartialFailureResult.h"

namespace railroad {

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bind(std::function<
     Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
         nakedFunc) {
  return nakedFunc;
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bind(std::function<
     PartialSuccessResult<OutputType>(PartialSuccessResult<InputType>)>
         nakedFunc) {
  return binds(nakedFunc);
}

template <typename OutputType, typename InputType, typename OutputFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
bind(std::function<
     PartialFailureResult<OutputFailureType>(PartialFailureResult<InputFailureType>)>
         nakedFunc) {
  return bindf(nakedFunc);
}

}  // namespace railroad