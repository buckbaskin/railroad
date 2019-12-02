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
#include "railroad/is_instantiation.h"
#include "railroad/rbind.h"

namespace railroad {

using ::railroad::PartialFailureResult;
using ::railroad::PartialSuccessResult;

template <typename OutputType, typename HiddenType, typename InputType>
std::function<OutputType(InputType)> operator>>(
    std::function<HiddenType(InputType)> inner,
    std::function<OutputType(HiddenType)> outer) {
  return [inner, outer](InputType input) { return outer(inner(input)); };
}

// Generic >>=

template <typename OutputType, typename HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType,
          typename OuterFunc>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    OuterFunc outer) {
  static_assert(is_instantiation<std::function, OuterFunc>::value,
                "Need OuterFunc to be an instantiation of std::function");
  return inner >>
         rbind<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

// Specialization for 2:2 >>= 2:2

template <typename OutputType, typename HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    std::function<Result<OutputType, OutputFailureType>(
        Result<HiddenType, HiddenFailureType>)>
        outer) {
  return inner >> outer;
}

// Specialization for 2:2 >>= 1:2

template <typename OutputType, typename HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    std::function<
        Result<OutputType, OutputFailureType>(PartialSuccessResult<HiddenType>)>
        outer) {
  return inner >>
         rbind<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

template <typename OutputType, typename HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    std::function<Result<OutputType, OutputFailureType>(
        PartialFailureResult<HiddenFailureType>)>
        outer) {
  return inner >>
         rbind<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

// Specialization for 2:1 >>= 2:2

}  // namespace railroad
