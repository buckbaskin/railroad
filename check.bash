HEADER_FILES=$(ag --cpp -l -g ".*" include/)
SOURCE_FILES=$(ag --cpp -l -g ".*" src/)
TEST_FILES=$(ag --cpp -l -g ".*" test/)
ALL_FILES=$(echo ${HEADER_FILES} ${SOURCE_FILES} ${TEST_FILES})

clang-check-6.0 -p build/compile_commands.json ${SOURCE_FILES} ${TEST_FILES} &&\
cppcheck --language=c++ --std=c++14 ${ALL_FILES} &&\
cpplint ${ALL_FILES}
