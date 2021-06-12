#!/bin/bash

comp=gcc
src='src/*.c src/scripts/*.c src/UI/*.c'
exe=tree_game
std=-std=c99
dir=~/Dev/Engine/Heart/

flags=(
    $std
    -Wall
    -Wextra
    -O2
    #-fsyntax-only
    #-fsanitize=address
)

inc=(
    '-I.'
    '-I'$dir
    '-I'$dir'src/'
    '-I'$dir'include/'
    '-L'$dir'lib/'
)

lib=(
    -lglfw
    -lpng
    -lfreetype
    -lenet
    -lfract
    -lutopia
    -lcore
    -ltaxi
    -lnano
    -limagine
    -lphoton
    -ldwarf
    -lzbug
    -lspawn
    -lmodular
    -luniverse
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


compile() {
    if echo "$OSTYPE" | grep -q "darwin"; then
        $comp ${flags[*]} ${inc[*]} ${lib[*]} ${mac[*]} $src -o $dir'bin/'$exe
    elif echo "$OSTYPE" | grep -q "linux"; then
        $comp ${flags[*]} ${inc[*]} ${lib[*]} ${linux[*]} $src -o $dir'bin/'$exe
    else
        echo "OS is not supported yet..."
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

if [[ $# < 1 ]]
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
