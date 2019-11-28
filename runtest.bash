# runtest.bash
cat runtest.bash

./compile.bash &&\

cd build/ &&\
make all-tests &&\
ctest -V
CTEST_STATUS=$?
cd -

echo CTEST_STATUS ${CTEST_STATUS}
exit ${CTEST_STATUS}
