/*
 * style.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <functional>

#include "railroad/DefaultFailure.h"
#include "railroad/DefaultSuccess.h"
#include "railroad/Result.h"

namespace railroad {
template <typename OutputType, typename HiddenType, typename InputType>
std::function<OutputType(InputType)> operator>>(
    std::function<HiddenType(InputType)> inner,
    std::function<OutputType(HiddenType)> outer) {
  return [inner, outer](InputType input) { return outer(inner(input)); };
}

template <typename OutputType, typename HiddenType, typename InputType,
          typename OutputFailureType, typename HiddenFailureType,
          typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
operator>>=(std::function<HiddenType(InputType)> inner,
            std::function<Result<OutputType, OutputFailureType>(
                Result<HiddenType, HiddenFailureType>)>
                outer) {
  auto boundInner = binds(inner);
  return
      [boundInner, outer](InputType input) { return outer(boundInner(input)); };
}

template <typename OutputType, typename HiddenType, typename InputType,
          typename OutputFailureType = ::railroad::DefaultFailure,
          typename HiddenFailureType, typename InputFailureType>
std::function<
    Result<OutputType, OutputFailureType>(Result<InputType, InputFailureType>)>
operator>>=(std::function<Result<HiddenType, HiddenFailureType>(
                Result<InputType, InputFailureType>)>
                inner,
            std::function<OutputType(HiddenType)> outer) {
  auto boundOuter = binds(outer);
  return [inner, boundOuter](Result<InputType, InputFailureType> input) {
    return boundOuter(inner(input));
  };
}

}  // namespace railroad
