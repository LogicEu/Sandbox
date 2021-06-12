#!/bin/bash

app='modular_ecs_test'
dir=/Users/eugenio/Dev/Engine/Heart/
lib=(
   '-I'$dir'include/'
   '-I'$dir'src/'
   '-L'$dir'lib/'
    -framework OpenGL
    -lmodular
    -lfract
    -limagine
    -lzbug
    -lcore
    -ltaxi
    -lpng
    -lglfw
)

comp() {
    gcc -std=c99 -Wall -O2 ${lib[*]} example.c -o $dir'bin/'$app
}

exe() {
    cd ~/Dev/Engine/Heart/
    ./shell.sh exe $app "$@"
}

run() {
    comp
    exe "$@"
}

if (($# >= 1))
then
    if [[ "$1" == "run" ]]
    then
        shift
        run "$@"
        exit
    elif [[ "$1" == "comp" ]]
    then
        comp
        exit
    elif [[ "$1" == "exe" ]]
    then
        shift
        exe "$@"
        exit
    fi
fi

echo "Use 'comp' to compile, 'exe' to execute or 'run' to do both."
