/*
 * GenericCompose.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <iostream>

#include "railroad/ControlFlags.h"
#include "railroad/abc.h"

namespace railroad {
namespace bind {
namespace generic {

template <typename OutputType, typename HiddenType, typename InputType>
class GenericCompose
    : public ::railroad::abc::Callable1<OutputType, InputType> {
  using InnerCallable = ::railroad::abc::Callable1<HiddenType, InputType>;
  using OuterCallable = ::railroad::abc::Callable1<OutputType, HiddenType>;

 public:
  explicit GenericCompose(const InnerCallable& inner,
                          const OuterCallable& outer)
      : inner_(inner), outer_(outer) {}

  OutputType operator()(const InputType& input) const override {
    return outer_(inner_(input));
  }

  template <typename OtherOutput>
  GenericCompose<OtherOutput, OutputType, InputType> operator>>(
      const ::railroad::abc::Callable1<OtherOutput, OutputType>& newOuter) {
    if (ControlFlags::LOG_DEBUG) {
      std::cout << "GenericCompose operator>>(lhs, Callable newOuter)"
                << std::endl;
    }
    return GenericCompose<OtherOutput, OutputType, InputType>{*this, newOuter};
  }

 private:
  const InnerCallable& inner_;
  const OuterCallable& outer_;
};

template <typename OutputType, typename HiddenType, typename InputType>
GenericCompose<OutputType, HiddenType, InputType> operator>>(
    const ::railroad::abc::Callable1<HiddenType, InputType>& inner,
    const ::railroad::abc::Callable1<OutputType, HiddenType>& outer) {
  if (ControlFlags::LOG_DEBUG) {
    std::cout << "GenericCompose operator>>(Callable inner, Callable outer)"
              << std::endl;
  }
  return GenericCompose{inner, outer};
}

}  // namespace generic
}  // namespace bind
}  // namespace railroad
