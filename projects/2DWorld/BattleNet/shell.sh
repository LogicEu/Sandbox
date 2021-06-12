#!/bin/bash

common_src='common/*.c'
client_src='client/client.c client/src/*.c'
editor_src='client/editor.c client/src/*.c'
server_src='server/*.c'

comp=gcc
src=src/*.c
exe=battle_net
std=-std=c99
dir=/Users/eugenio/Dev/Engine/Heart/

flags=(
    $std
    -Wall
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
    -framework OpenGL
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
)

compile() {
    $comp ${flags[*]} ${inc[*]} ${lib[*]} $src -o $dir'bin/'$exe
}

execute() {
    cd $dir
    ./shell.sh exe $exe "$@"
}

opboth() {
    src="$client_src $common_src"
    compile
    src="$server_src $common_src"
    compile
    exit
}

fail() {
    echo "Use 'client', 'server' or 'both'."
    echo "Use 'comp' to compile, 'exe' to execute or 'run' to do both."
    exit
}

if [[ $# < 1 ]]
then
    fail
fi

if [[ "$1" == "client" ]]
then
    shift
    exe=BNClient
    src="$client_src $common_src"
elif [[ "$1" == "editor" ]]
then
    shift
    src="$editor_src $common_src"
    exe=BNEditor
elif [[ "$1" == "server" ]]
then
    shift
    src="$server_src $common_src"
    exe=BNServer
elif [[ "$1" == "both" ]]
then
    shift
    opboth
else
    fail
fi

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
