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
#include "railroad/bind.h"
#include "railroad/is_instantiation.h"

namespace railroad {
template <typename OutputType, typename HiddenType, typename InputType>
std::function<OutputType(InputType)> operator>>(
    std::function<HiddenType(InputType)> inner,
    std::function<OutputType(HiddenType)> outer) {
  return [inner, outer](InputType input) { return outer(inner(input)); };
}

template <typename OutputType, typename HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType,
          typename OuterFunc>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    OuterFunc outer) {
  static_assert(is_instantiation<std::function, OuterFunc>::value,
                "Need OuterFunc to be an instantiation of std::function");
  return inner >>
         bindr<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

template <typename OutputType, typename HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    std::function<Result<OutputType, OutputFailureType>(
        Result<HiddenType, HiddenFailureType>)>
        outer) {
  return inner >>
         bindr<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

template <typename OutputType, typename HiddenType, typename T,
          typename HiddenFailureType,
          typename OutputFailureType = HiddenFailureType>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    std::function<
        PartialSuccessResult<OutputType>(PartialSuccessResult<HiddenType>)>
        outer) {
  return inner >>
         bindr<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

template <typename HiddenType, typename OutputType = HiddenType, typename T,
          typename OutputFailureType, typename HiddenFailureType>
std::function<Result<OutputType, OutputFailureType>(T)> operator>>=(
    std::function<Result<HiddenType, HiddenFailureType>(T)> inner,
    std::function<PartialFailureResult<OutputFailureType>(
        PartialSuccessResult<HiddenType>)>
        outer) {
  return inner >>
         bindr<OutputType, HiddenType, OutputFailureType, HiddenFailureType>(
             outer);
}

template <typename InputType, typename InputFailureType>
std::function<
    Result<InputType, InputFailureType>(Result<InputType, InputFailureType>)>
operator>>=(std::function<PartialFailureResult<InputFailureType>(
                PartialSuccessResult<InputType>)>
                inner,
            std::function<PartialFailureResult<InputFailureType>(
                PartialSuccessResult<InputType>)>
                outer) {
  return bindr<InputType, InputType, InputFailureType, InputFailureType>(inner);
}

}  // namespace railroad
