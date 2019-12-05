# compile.bash
cat compile.bash

mkdir -p build/ &&\
cd build/ &&\
CXX=/usr/bin/clang++-8 CC=clang-8 cmake .. &&\
make -j4 &&\
# cmake --build . &&\
cd ..

