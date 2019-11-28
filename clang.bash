# compile.bash
cat compile.bash

mkdir -p build/ &&\
cd build/ &&\
CXX=clang++-6.0 CC=clang-6.0 cmake .. &&\
cmake --build . &&\
cd ..

