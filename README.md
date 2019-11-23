# Railroad

Header-only library for bringing Railway Oriented Programming to C++

[![Build Status](https://travis-ci.com/buckbaskin/railroad.svg?branch=master)](https://travis-ci.com/buckbaskin/railroad)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/3427/badge)](https://bestpractices.coreinfrastructure.org/projects/3427)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/buckbaskin/railroad.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/buckbaskin/railroad/context:cpp)

## What is it?

Nicer looking function composition that makes it easy to explicitly handle errors

```C++
auto result =
    (feed
      >> binds(adder)
      >> binds(adder)
      >> binds<int, int>([](int i) { return i - 1; })
      >> binds(stringify) >>
      terminate)(0);
```

The syntax and concept were inspired by [Scott Wlaschin's presentation about the idea in F#](https://fsharpforfunandprofit.com/rop/).

## Example

### Code

Based on `src/railroad/main.cpp`

```C++
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "railroad/railroad.h"

using ::railroad::binds;  // aka bindSuccess

int main(int /* argc */, char** /* argv */) {
  std::function<int(int)> adder = [](int i) { return i + 1; };
  std::function<std::string(int)> stringify = [](int i) {
    std::stringstream buf;
    buf << i << " ;) ";
    return buf.str();
  };

  std::function feed = ::railroad::helpers::feedSuccess<int>;
  std::function terminate = ::railroad::helpers::terminateSuccess<std::string>;

  std::cout << "\">> Composed\" Functions:" << std::endl <<
           *((feed
              >> binds(adder)
              >> binds(adder)
              >> binds<int, int>([](int i) { return i - 1; })
              >> binds(stringify) >>
              terminate)(41))
            << std::endl;
  return 0;
}
```

Output:

```bash
~/railroad$ ./run.bash
[ 50%] Building CXX object CMakeFiles/demonstrator.dir/src/railroad/main.cpp.o
[100%] Linking CXX executable demonstrator
[100%] Built target demonstrator
">> Composed" Functions:
You can do anything 42 time(s)
```

### Compiling

```bash
~/railroad$ ./compile.bash
```

## Documentation

https://buckbaskin.com/railroad/docs/

## Open TODOs

See [Github Issues](https://github.com/buckbaskin/railroad/issues) for the latest status

- [ ] `operator>>=` for automatic binding (In Progress)
- [ ] Profiling/Performance Review
