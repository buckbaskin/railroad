/*
 * bind.h
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
class Noop : public ::railroad::abc::Callable1<
                 Result<OutputSuccessType, OutputFailureType>,
                 Result<InputSuccessType, InputFailureType>> {
 public:
  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    return input;
  }
};

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
  // Compose(const Callable& start)
  //     : inner_(Noop<InputSuccessType, OutputSuccessType, InputFailureType,
  //                   OutputFailureType>{}),
  //       outer_(start) {}
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
/*
template <typename InputSuccessType,
          typename InputFailureType, typename OutputType>
class TerminateCompose : public ::railroad::abc::Callable1<
                    OutputType,
                    Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<OutputType,
                                 Result<InputSuccessType, InputFailureType>>;

 public:
  explicit TerminateCompose(const Callable& inner, const Callable& outer)
      : inner_(inner), outer_(outer) {}

  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    return outer_(inner_(input));
  }

 private:
  const Callable& inner_;
  const Callable& outer_;
};

// clang-format off
template <typename InputSuccessType, typename InputFailureType,
          typename OutputType>
TerminateCompose<InputSuccessType, InputFailureType, OutputType>
operator>>(const ::railroad::abc::Callable1<
                    Result<OutputSuccessType, OutputFailureType>,
                    Result<InputSuccessType, InputFailureType>>& inner,
           const ::railroad::abc::Callable1<
                    OutputType,
                    Result<InputSuccessType, InputFailureType>>& outer) {
  return TerminateCompose{inner, outer};
}
// clang-format on
*/

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
