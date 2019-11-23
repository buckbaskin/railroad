# compile.bash
cat compile.bash

mkdir -p build/ &&\
cd build/ &&\
cmake --build . &&\
cd ..

