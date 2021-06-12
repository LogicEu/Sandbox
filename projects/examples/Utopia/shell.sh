#!/bin/bash

app='utopia_test'
dir=../../../
lib=(
   '-I'$dir'src/'
   '-L'$dir'lib/'
   -lutopia
)

comp() {
    gcc -std=c99 -Wall -O2 ${lib[*]} example.c -o $dir'bin/'$app
}

exe() {
    cd $dir
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
