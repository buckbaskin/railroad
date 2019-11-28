# format.bash
cat format.bash

clang-format-6.0 -style=file -i $(ag --cpp -l -g ".*" include/ src/ test/ example/ docs/)

