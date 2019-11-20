#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "railroad/abc.h"

class Increment : public ::railroad::abc::Callable1<int, int> {
 public:
  virtual int operator()(const int& input) const override { return input + 1; }
};

std::ostream& operator<<(std::ostream& out, const Increment& /* inc */) {
  out << "Increment__function(int) -> int + 1";
  return out;
}

template <typename SuccessType>
class PartialSuccessResult {
 public:
  PartialSuccessResult() : impl_(nullptr) {}
  PartialSuccessResult(const SuccessType& example)
      : PartialSuccessResult(new SuccessType(example)) {}
  PartialSuccessResult(SuccessType* impl) : impl_(impl) {}

  explicit operator bool() const { return static_cast<bool>(impl_); }

  SuccessType unpack() const { return *impl_; }

 private:
  std::shared_ptr<SuccessType> impl_;
};

template <typename FailureType>
class PartialFailureResult {
 public:
  PartialFailureResult() : impl_(nullptr) {}
  PartialFailureResult(const FailureType& example)
      : PartialFailureResult(new FailureType(example)) {}
  PartialFailureResult(FailureType* impl) : impl_(impl) {}

  explicit operator bool() { return static_cast<bool>(impl_); }

  FailureType unpack() const { return *impl_; }

 private:
  std::shared_ptr<FailureType> impl_;
};

enum class RailroadPath { SUCCESS, FAILURE };

template <typename SuccessType, typename FailureType>
class Result {
 public:
  Result() = delete;
  Result(const PartialSuccessResult<SuccessType>& success,
         const PartialFailureResult<FailureType>& failure)
      : success_(success), failure_(failure) {}
  bool hasSuccess() const { return static_cast<bool>(success_); }
  PartialSuccessResult<SuccessType> getSuccess() const { return success_; }
  bool hasFailure() const { return static_cast<bool>(failure_); }
  PartialFailureResult<FailureType> getFailure() const { return failure_; }

  static Result<SuccessType, FailureType> Success(
      const PartialSuccessResult<SuccessType>& happy) {
    return {happy, PartialFailureResult<FailureType>{}};
  }
  static Result<SuccessType, FailureType> Success(const SuccessType& happy) {
    return {PartialSuccessResult<SuccessType>{happy},
            PartialFailureResult<FailureType>{}};
  }
  static Result<SuccessType, FailureType> Failure(
      const PartialFailureResult<FailureType>& sad) {
    return {PartialSuccessResult<SuccessType>{}, sad};
  }
  static Result<SuccessType, FailureType> Failure(const FailureType& sad) {
    return {PartialSuccessResult<SuccessType>{},
            PartialFailureResult<FailureType>{sad}};
  }

 private:
  PartialSuccessResult<SuccessType> success_;
  PartialFailureResult<FailureType> failure_;
};

template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType = std::string,
          typename OutputFailureType = std::string>
class BindSuccessPipe : public ::railroad::abc::Callable1<
                            Result<OutputSuccessType, OutputFailureType>,
                            Result<InputSuccessType, InputFailureType>> {
  using Callable =
      ::railroad::abc::Callable1<OutputSuccessType, InputSuccessType>;

 public:
  BindSuccessPipe(const Callable& impl) : impl_(impl) {}

  virtual Result<OutputSuccessType, OutputFailureType> operator()(
      const Result<InputSuccessType, InputFailureType>& input) const override {
    if (input.hasSuccess()) {
      return Result<OutputSuccessType, OutputFailureType>::Success(
          impl_(input.getSuccess().unpack()));
    } else {
      return Result<OutputSuccessType, OutputFailureType>::Failure(
          input.getFailure());
    }
  }

 private:
  const Callable& impl_;
};

int main(int /* argc */, char** /* argv */) {
  int rawInput = 0;

  Increment successOnlyFunction;
  int directOutput = successOnlyFunction(successOnlyFunction(rawInput));

  Result<int, std::string> input = Result<int, std::string>::Success(rawInput);
  BindSuccessPipe railroadIncrementFunction{successOnlyFunction};
  Result<int, std::string> output =
      railroadIncrementFunction(railroadIncrementFunction(input));

  int railroadOutput = output.getSuccess().unpack();

  if (directOutput == railroadOutput) {
    std::cout << "Successfully bound " << successOnlyFunction << " to get "
              << railroadOutput << " from " << rawInput << std::endl;
  } else {
    std::cout << "Binding failed for " << successOnlyFunction << " to get "
              << railroadOutput << " from " << rawInput << std::endl;
  }

  return 0;
}
