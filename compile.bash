# compile.bash
cat compile.bash

mkdir -p build/ &&\
cd build/ &&\
cmake .. &&\
cmake --build . &&\
cd ..

