# runtest.bash
cat runtest.bash

./compile.bash &&\

cd build/ &&\
make all-tests &&\
RC_PARAMS="seed=12776003016957408636" ctest -V
CTEST_STATUS=$?
cd -

echo CTEST_STATUS ${CTEST_STATUS}
exit ${CTEST_STATUS}
