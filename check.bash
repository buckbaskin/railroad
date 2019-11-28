# check.bash
cat check.bash

HEADER_FILES=$(ag --cpp -l -g ".*" include/)
SOURCE_FILES=$(ag --cpp -l -g ".*" src/ example/)
TEST_FILES=$(ag --cpp -l -g ".*" test/)
ALL_FILES=$(echo ${HEADER_FILES} ${SOURCE_FILES} ${TEST_FILES})

./format.bash

echo "Begin Checks"

clang-check-6.0 -p build/compile_commands.json ${SOURCE_FILES} ${TEST_FILES} &&\
echo "Done clang-check" &&\
clang-tidy-6.0 -checks=-*,boost*,bugprone*,clang-analyzer*,-clang-analyzer-osx*,cpp-core-guidelines*,google*,hicpp*,llvm*,misc*,modernize*,performance*,readability*,-readability/check -p build/compile_commands.json ${SOURCE_FILES}
echo "Done clang checks"

cppcheck --language=c++ --std=c++14 ${ALL_FILES}
cpplint --linelength=120 ${ALL_FILES}

