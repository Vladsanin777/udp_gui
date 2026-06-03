export LANG=C.UTF-8
export LC_ALL=C.UTF-8

rm -rf build && mkdir build && cd build && cmake .. \
        && cmake --build . && cmake --build . --target update_translations
