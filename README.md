# Railroad

Header-only library for bringing Railway Oriented Programming to C++

## Example

### Code

Based on `src/railroad/main.cpp`

```C++
#include <iostream>
#include <sstream>
#include <string>

#include "railroad/FailurePipe.h"
#include "railroad/Result.h"
#include "railroad/SuccessPipe.h"
#include "railroad/ValidateSuccess.h"
#include "railroad/abc.h"
#include "railroad/bind.h"

class Increment : public ::railroad::abc::Callable1<int, int> {
 public:
  int operator()(const int& input) const override { return input + 1; }
};

class RangeCheck
    : public ::railroad::abc::Callable1<::railroad::Result<int, std::string>,
                                        int> {
  using OutputResult = ::railroad::Result<int, std::string>;

 public:
  OutputResult operator()(const int& input) const override {
    if (0 <= input && input <= 1) {
      return OutputResult::Success(input);
    } else {
      std::stringstream buf;
      buf << "Integer Input Out of Range: " << input << " not in [0, 1]";
      return OutputResult::Failure(buf.str());
    }
  }
};

using namespace ::railroad;
using InputResult = Result<int, std::string>;

int main() {

  int rawInput = 0;

  Increment successOnlyFunction;
  ::railroad::bind::SuccessPipe railroadIncrementFunction{successOnlyFunction};

  RangeCheck validateAndSplit;
  ::railroad::bind::ValidateSuccess railroadFromSplit(validateAndSplit);

  int fancyComposedResult =
      (railroadIncrementFunction
        >> railroadFromSplit
        >> railroadIncrementFunction
        >> railroadIncrementFunction)(InputResult::Success(rawInput)).getSuccess().unpack();

  std::cout << "Got result " << fancyComposedResult
            << " via syntax composition." << std::endl;

}
```

Output:

```bash
~/railroad$ ./run.bash
[ 50%] Building CXX object CMakeFiles/demonstrator.dir/src/railroad/main.cpp.o
[100%] Linking CXX executable demonstrator
[100%] Built target demonstrator
Got result 3 via syntax composition.
Got result 3 via syntax composition.
```

### Compiling

```bash
~/railroad$ ./compile.bash
```
