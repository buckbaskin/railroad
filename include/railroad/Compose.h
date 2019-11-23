/*
 * Compose.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <string>

#include "railroad/FailurePipe.h"
#include "railroad/Result.h"
#include "railroad/SuccessPipe.h"
#include "railroad/ValidateSuccess.h"
#include "railroad/abc.h"

namespace railroad {
namespace bind {

template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType, typename OutputFailureType>
class Compose : public ::railroad::abc::Callable1<
                    Result<OutputSuccessType, OutputFailureType>,
                    Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<Result<OutputSuccessType, OutputFailureType>,
                                 Result<InputSuccessType, InputFailureType>>;

 public:
  Compose()
      : inner_(Noop<InputSuccessType, OutputSuccessType, InputFailureType,
                    OutputFailureType>{}),
        outer_(Noop<InputSuccessType, OutputSuccessType, InputFailureType,
                    OutputFailureType>{}) {}
  explicit Compose(const Callable& inner, const Callable& outer)
      : inner_(inner), outer_(outer) {}

  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    return outer_(inner_(input));
  }

  Compose operator>>(const Callable& newOuter) {
    return Compose(*this, newOuter);
  }

 private:
  const Callable& inner_;
  const Callable& outer_;
};

// clang-format off
template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType, typename OutputFailureType>
Compose<InputSuccessType, OutputSuccessType,
        InputFailureType, OutputFailureType>
operator>>(const ::railroad::abc::Callable1<
                    Result<OutputSuccessType, OutputFailureType>,
                    Result<InputSuccessType, InputFailureType>>& inner,
           const ::railroad::abc::Callable1<
                    Result<OutputSuccessType, OutputFailureType>,
                    Result<InputSuccessType, InputFailureType>>& outer) {
  return Compose{inner, outer};
}
// clang-format on

}  // namespace bind
}  // namespace railroad
