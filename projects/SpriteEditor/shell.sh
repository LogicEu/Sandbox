#!/bin/bash

comp=gcc
src=*.c
exe=sprite_editor
std=-std=c99
dir=/Users/eugenio/Dev/Engine/Heart/

flags=(
    $std
    -Wall
    #-Wextra
    -O2
    -mmacos-version-min=10.9
)

inc=(
    '-I'$dir'src/'
    '-I'$dir'include/'
    '-I'$dir'include/FreeType/'
    '-I'$dir'include/FMOD/'
    '-L'$dir'lib/'
)

lib=(
    -framework OpenGL
    -lcore
    -limagine
    -lglfw
    -lpng
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
