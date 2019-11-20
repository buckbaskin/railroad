/*
 * PartialSuccessResult.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <memory>

namespace railroad {
namespace result {

template <typename SuccessType>
class PartialSuccessResult {
 public:
  PartialSuccessResult() : impl_(nullptr) {}
  explicit PartialSuccessResult(const SuccessType& example)
      : PartialSuccessResult(new SuccessType(example)) {}
  explicit PartialSuccessResult(SuccessType* impl) : impl_(impl) {}

  explicit operator bool() const { return static_cast<bool>(impl_); }

  SuccessType unpack() const { return *impl_; }

 private:
  std::shared_ptr<SuccessType> impl_;
};

}  // namespace result
}  // namespace railroad
