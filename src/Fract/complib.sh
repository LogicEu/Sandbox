#!/bin/bash

name=libfract

flags=(
    -std=c99
    -Wall
    -Wextra
    -O2
)

inc=(
    -I.
    -I../../
    -I../../src/
)

fail_op() {
    echo "Run with -d to build dynamically, or -s to build statically."
    exit
}

fail_os() {
    echo "OS is not supported yet..."
    exit
}

mac_dlib() {
        gcc ${flags[*]} ${inc[*]} ${lib[*]} -dynamiclib src/*.c -o ../../lib/$name.dylib
        install_name_tool -id @executable_path/lib/$name.dylib ../../lib/$name.dylib 
}

linux_dlib() {
    gcc -shared ${flags[*]} ${inc[*]} ${lib[*]} -lm -fPIC src/*.c -o ../../lib/$name.so 
}

dlib() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        mac_dlib
    elif echo "$OSTYPE" | grep -q "linux"; then
        linux_dlib
    else
        fails_os
    fi
}

slib() {
    gcc ${flags[*]} ${inc[*]} -c src/*.c
    ar -crv ../../lib/$name.a *.o
    rm *.o
}

if [[ $# < 1 ]]
then 
    fail_op
fi

if [[ "$1" == "-d" ]]
then
    dlib
elif [[ "$1" == "-s" ]]
then
    slib
else
    fail_op
fi 
