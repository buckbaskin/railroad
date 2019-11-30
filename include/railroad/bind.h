/*
 * binds.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/Result.h"

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

}  // namespace railroad