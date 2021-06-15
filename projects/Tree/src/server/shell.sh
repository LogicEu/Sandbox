#!/bin/bash

comp=gcc
src='*.c'
exe=tree_server
std=-std=c99
dir=../../../../

flags=(
    $std
    -Wall
    -Wextra
    -O2
)

inc=(
    '-I.'
    '-I'$dir
    '-I'$dir'src/'
    '-I'$dir'include/'
    '-L'$dir'lib/'
)

lib=(
    -lenet
    -lutopia
    -lnano
    -lzbug
)


compile() {
    $comp ${flags[*]} ${inc[*]} ${lib[*]} ${mac[*]} $src -o $dir'bin/'$exe
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
