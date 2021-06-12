#!/bin/bash

comp=gcc
src=*.c
exe=sprite_editor
std=-std=c99
dir=../../

flags=(
    $std
    -Wall
    #-Wextra
    -O2
)

inc=(
    '-I'$dir'src/'
    '-I'$dir'include/'
    '-I'$dir'include/FreeType/'
    '-I'$dir'include/FMOD/'
    '-L'$dir'lib/'
)

lib=(
    -lcore
    -limagine
    -lglfw
    -lpng
    -ljpeg
)

linux=(
    -lGL
    -lGLEW
)

mac=(
    -framwork OpenGL
    -mmacos-version-min=10.9
)

compile() {
    if echo "$OSTYPE" | grep -q "linux"; then
        $comp ${flags[*]} ${inc[*]} ${lib[*]} ${linux[*]} $src -o $dir'bin/'$exe
    elif echo "$OSTYPE" | grep -q "darwin"; then
        $comp ${flags[*]} ${inc[*]} ${lib[*]} ${mac[*]} $src -o $dir'bin/'$exe
    else
        echo "OS not supported yet..."
        exit
    fi
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
