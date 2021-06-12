#!/bin/bash

characters=(
    Victoria
    Emilia
    Diego
    Leonardo
    Paula
    Sofía
    Pedro
    Damián
    Cecilia
    Sara
    Itzel
)

op() {
    open desglose.xlsx
}

echo "Testing movietool"
pushd ../../
./shell.sh exe movietool assets/pdf/ER.pdf ${characters[*]}

if (( $# >= 1 ))
then
    if [[ "$1" == "open" ]]
    then
        op
    fi
fi

popd
