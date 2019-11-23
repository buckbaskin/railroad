/*
 * SuccessPipe.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <iostream>

#include "railroad/DefaultFailure.h"
#include "railroad/Result.h"
#include "railroad/abc.h"

namespace railroad {
namespace bind {

template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType = ::railroad::DefaultFailure,
          typename OutputFailureType = ::railroad::DefaultFailure>
class SuccessPipe : public ::railroad::abc::Callable1<
                        Result<OutputSuccessType, OutputFailureType>,
                        Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<OutputSuccessType, InputSuccessType>;

 public:
  explicit SuccessPipe(const Callable& impl) : impl_(impl) {
    std::cout << "SuccessPipe constructed" << std::endl;
  }

  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    if (input.hasSuccess()) {
      std::cout << "SuccessPipe<> input hasSuccess " << input.getSuccess()
                << std::endl;
      return Result<OutputSuccessType, OutputFailureType>::Success(
          impl_(input.getSuccess()), input.getFailurePartial());
    } else {
      std::cout << "SuccessPipe<> input isFailing " << std::endl;
      return Result<OutputSuccessType, OutputFailureType>::Failure(
          input.getFailurePartial());
    }
  }

 private:
  const Callable& impl_;
};

}  // namespace bind
}  // namespace railroad
