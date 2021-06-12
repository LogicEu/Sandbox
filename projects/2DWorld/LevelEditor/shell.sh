#!/bin/bash

comp=gcc
src=*.c
exe=level_editor
std=-std=c99
dir=../../../

flags=(
    $std
    -Wall
    -O2
)

inc=(
    '-I'$dir
    '-I'$dir'src/'
    '-I'$dir'include/'
    '-L'$dir'lib/'
)

lib=(
    -lglfw
    -lpng
    -lfreetype
    -lfract
    -lutopia
    -lcore
    -ltaxi
)

mac=(
    -framework OpenGL
    -mmacos-version-min=10.9
)

linux=(
    -lm
    -lGL
    -lGLEW
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
