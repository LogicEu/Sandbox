#!/bin/bash

name=imgtool

flags=(
    -Wall
    -Wextra
    -O2
    -std=c99
)

inc=(
    -L../../lib/
    -I.
    -I../../src/
    -I../../include/
    -lpng
    -ljpeg
    -limagine
)

comp() {
    echo "Compiling $name"
    gcc ${flags[*]} ${inc[*]} *.c -o ../../bin/$name
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
