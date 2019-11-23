/*
 * bind.h
 *
 * Copyright 2019 Buck Baskin
 */

#pragma once

#include <string>

#include "railroad/DefaultFailure.h"
#include "railroad/PartialSuccessPipe.h"
#include "railroad/Result.h"
#include "railroad/SuccessPipe.h"
#include "railroad/abc.h"

namespace railroad {

using bind::PartialSuccessPipe;
using bind::SuccessPipe;

// template <typename InputSuccessType, typename OutputSuccessType,
//           typename InputFailureType = ::railroad::DefaultFailure,
//           typename OutputFailureType = ::railroad::DefaultFailure>
// PartialSuccessPipe<InputSuccessType, OutputSuccessType, InputFailureType,
//                    OutputFailureType>
// binds(const
// ::railroad::abc::Callable1<PartialSuccessResult<OutputSuccessType>,
//                                        PartialSuccessResult<InputSuccessType>>&
//           bindThis) {
//   return PartialSuccessPipe(bindThis);
// }

template <typename InputSuccessType, typename OutputSuccessType,
          typename InputFailureType = ::railroad::DefaultFailure,
          typename OutputFailureType = ::railroad::DefaultFailure>
SuccessPipe<InputSuccessType, OutputSuccessType, InputFailureType,
            OutputFailureType>
binds(const ::railroad::abc::Callable1<OutputSuccessType, InputSuccessType>&
          bindThis) {
  return SuccessPipe(bindThis);
}

}  // namespace railroad
