# compile.bash
cat compile.bash

mkdir -p build/ &&\
cd build/ &&\
/usr/bin/cmake --build . &&\
cd ..

