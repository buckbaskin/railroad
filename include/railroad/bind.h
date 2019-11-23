/*
 * bind.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <string>

#include "railroad/Compose.h"
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
class TerminateSuccessCompose
    : public ::railroad::abc::Callable1<
          OutputSuccessType, Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<Result<OutputSuccessType, OutputFailureType>,
                                 Result<InputSuccessType, InputFailureType>>;
  using TerminateCallable =
      ::railroad::abc::Callable1<OutputSuccessType,
                                 Result<InputSuccessType, InputFailureType>>;

 public:
  explicit TerminateSuccessCompose(const Callable& inner,
                                   const Callable& terminator)
      : inner_(inner), terminator_(terminator) {}

  virtual OutputSuccessType operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    return terminator_(inner_(input));
  }

 private:
  const Callable& inner_;
  const Callable& terminator_;
};

// TODO: Start here w/ operator>> to take a (callable, TerminateCallable) ->
// TerminateSuccessCompose

}  // namespace bind
}  // namespace railroad
