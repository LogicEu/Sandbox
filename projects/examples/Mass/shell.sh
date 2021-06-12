#!/bin/bash

app='obj_model_test'
dir=../../../
lib=(
   '-I'$dir'src/'
   '-L'$dir'lib/'
    -lmass
    -lfract
    -lutopia
)

comp() {
    if echo "$OSTYPE" | grep -q "linux"; then
        gcc -std=c99 -Wall -O2 ${lib[*]} -lm .c -o $dir'bin/'$app
    else
        gcc -std=c99 -Wall -O2 ${lib[*]} *.c -o $dir'bin/'$app
    fi
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
