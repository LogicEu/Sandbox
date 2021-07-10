#!/bin/bash

comp() {
    cd $1
    echo "Compiling '$1'"
    shift
    ./complib.sh "$@"
    cd ..
}

fail() {
    echo "Run with -d to build all libraries dynamically, or -s to build statically"
    exit
}

if [[ $# < 1 ]]
then
    fail
elif [[ "$1" != "-d" ]]
then
    if [[ "$1" != "-s" ]]
    then
        fail
    fi
fi

comp "ZBug" "$1"
comp "Utopia" "$1"
comp "Fract" "$1"
comp "Core" "$1"
comp "Aural" "$1"
comp "Imagine" "$1"
comp "Mass" "$1"
comp "Nano" "$1"
comp "Photon" "$1"
comp "Taxi" "$1"
comp "Dwarf" "$1"
comp "Spawn" "$1"
comp "Modular" "$1"
comp "Universe" "$1"
comp "Nerv" "$1"
echo "Libraried built succesfully. Process completed."
