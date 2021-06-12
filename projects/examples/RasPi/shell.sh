#!/bin/bash

app='RasPiGLtest'
dir=../../../
lib=(
    '-I'$dir'src/'
    '-L'$dir'lib/'
    -lglfw
    -lcore
)

linux=(
    -lGL
    -lGLEW
)

mac=(
    -framework OpenGL
    -mmacos-version-min=10.9
)

comp() {
    if echo "$OSTYPE" | grep -q "linux"; then
        gcc -std=c99 -Wall -Wextra -O2 ${lib[*]} ${linux[*]} *.c -o $dir'bin/'$app
    elif echo "$OSTYPE" | grep -q "darwin"; then
        gcc -std=c99 -Wall -Wextra -O2 ${lib[*]} ${mac[*]} *.c -o $dir'bin/'$app
    else
        echo "OS not supported yet..."
        exit
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
