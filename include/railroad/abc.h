/*
 * abc.h
 *
 * Borrow useful base classes from Python's abc (Abstract Base
 * Classes) module
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <iostream>
#include <memory>

namespace railroad {
namespace abc {
template <typename ReturnType>
class Callable0 {
 public:
  virtual ReturnType operator()() const { return ReturnType{}; }
};

template <typename ReturnType, typename InputType>
class Callable1 {
 public:
  virtual ReturnType operator()(const InputType& /* arg1 */) const {
    std::cout << "Callable1 default operator()" << std::endl;
    return ReturnType{};
  }
};

template <typename ReturnType, typename InputType1, typename InputType2>
class Callable2 {
 public:
  virtual ReturnType operator()(const InputType1& /* arg1 */,
                                const InputType2& /* arg2 */) const {
    return ReturnType{};
  }
};
}  // namespace abc
}  // namespace railroad
