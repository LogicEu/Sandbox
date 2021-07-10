#!/bin/bash

name=3Dpi

flags=(
    -Wall
    -Wextra
    -O2
    -std=c99
)

inc=(
    -L../../lib/
    -I.
    -I../../
    -I../../include/
    -I../../src/
    -lglfw
    -lzbug
    -lcore
    -lfract
    -lutopia
    -lmass
    -limagine
    -ltaxi
)

linux=(
    -lm
    -lGL
    -lGLEW
)

mac=(
    -framework OpenGL
    -mmacos-version-min=10.9
)

comp() {
    echo "Compiling $name"
    if echo "$OSTYPE" | grep -q "linux"; then
        gcc ${flags[*]} ${inc[*]} ${linux[*]} *.c -o ../../bin/$name
    elif echo "$OSTYPE" | grep -q "darwin"; then
        gcc ${flags[*]} ${inc[*]} ${mac[*]} *.c -o ../../bin/$name
    else 
        echo "OS not supported yet..."
        exit
    fi
}

exe() {
    echo "Executing $name"
    cd ../../
    ./shell.sh exe $name "$@"
}

fail() { 
    echo "Use 'comp', 'exe' or 'run' commands."
    exit
}

if [[ $# < 1 ]]
then 
    fail
fi

if [[ "$1" == "comp" ]]
then
    comp
elif [[ "$1" == "exe" ]]
then 
    shift
    exe "$@"
elif [[ "$1" == "run" ]]
then 
    shift
    comp
    exe "$@"
else 
    fail
fi
