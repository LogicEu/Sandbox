#!/bin/bash

comp=gcc
src=src/*.c
exe=LD48game
std=-std=c99
dir=/Users/eugenio/Dev/Engine/Heart/

flags=(
    $std
    -Wall
    -O2
)

inc=(
    '-I.'
    '-I'$dir'include/GLFW/'
    '-I'$dir'include/FreeType/'
    '-I'$dir'include/FMOD/'
    '-I'$dir'include/libpng/'
    '-L'$dir'lib/'
)

lib=(
    -framework OpenGL
    -lglfw
    -lpng
    -lfreetype
    -lfmod
)

compile() {
    $comp ${flags[*]} ${inc[*]} ${lib[*]} $src -o $dir'bin/'$exe
}

execute() {
    cd $dir
    ./shell.sh exe $exe "$@"
}

fail() {
    echo "Use 'comp' to compile, 'exe' to execute or 'run' to do both."
    exit
}

if [[ $# -eq 0 ]]
then
    fail
fi

if [[ "$1" == "run" ]]
then
    shift
    compile
    execute "$@"
elif [[ "$1" == "comp" ]]
then
    compile
elif [[ "$1" == "exe" ]]
then
    shift
    execute "$@"
else
    fail
fi
