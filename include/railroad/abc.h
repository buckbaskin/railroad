/*
 * abc.h
 *
 * Borrow useful base classes from Python's abc (Abstract Base
 * Classes) module
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

namespace railroad {
namespace abc {
template <typename ReturnType>
class Callable0 {
 public:
  virtual ReturnType operator()() const = 0;
};

template <typename ReturnType, typename InputType>
class Callable1 {
 public:
  virtual ReturnType operator()(const InputType& arg1) const = 0;
};

template <typename ReturnType, typename InputType1, typename InputType2>
class Callable2 {
 public:
  virtual ReturnType operator()(const InputType1& arg1,
                                const InputType2& arg2) const = 0;
};
}  // namespace abc
}  // namespace railroad
