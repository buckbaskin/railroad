# compile.bash
cat compile.bash

mkdir -p build/ &&\
cd build/ &&\
cmake .. &&\
make -j5 &&\
cd ..

