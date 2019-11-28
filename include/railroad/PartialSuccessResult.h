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

  template <typename S>
  friend std::ostream& operator<<(std::ostream& out,
                                  const PartialSuccessResult<S>& psr);

 private:
  std::shared_ptr<SuccessType> impl_;
};

template <typename SuccessType>
inline std::ostream& operator<<(std::ostream& out,
                                const PartialSuccessResult<SuccessType>& psr) {
  out << "Success?<>(";
  if (!static_cast<bool>(psr.impl_)) {
    out << "null";
  } else {
    out << *(psr.impl_);
  }
  out << ")";
  return out;
}

}  // namespace result
}  // namespace railroad
