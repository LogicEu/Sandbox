#!/bin/bash

app='audio_loop_test'
dir=../../../
lib=(
   '-I'$dir'src/'
   '-I'$dir'include/'
   '-L'$dir'lib/'
    -laural
    -lfmod
)

comp() {
    gcc -std=c99 -Wall -O2 ${lib[*]} *.c -o $dir'bin/'$app
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
