# Rationale and Design Decisions

A list of reasons for the design decisions that were made for the library.

## `std::function`

The original implementation of the library used an idea borrowed from Python called Callable that includes the return type and argument types in the template; however, this implementation proved cumbersome and verbose, so I opted to instead move to std::function.

std::function also allows for cleaner inter-operation with lambda, functors/operator(), function pointers and other C++ language constructs that represent some computation to be done.

