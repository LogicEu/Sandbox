#!/bin/bash

ldlp() {
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/:/usr/lib/arm-linux-gnueabihf/:lib/
    export LD_LIBRARY_PATH

}

exe() {
    if (($# == 0))
    then
        echo "Missing binary to execute"
        exit
    fi

    if echo "$OSTYPE" | grep -q "linux"; then
        ldlp
    fi

    exe=$1
    shift
    echo Running $exe "$@"
    cp bin/$exe .
    ./$exe "$@"
    rm $exe
}

build_mac_app() {
    bundle=App
    exe=app

    if (($# >= 2))
    then
        exe=$1
        bundle=$2
    elif (($# == 1))
    then
        exe=$1
        bundle=$1
    else
        echo "Missing input binary."
        exit
    fi

    echo Building $bundle.app

    mkdir $bundle.app/
    mkdir $bundle.app/Contents/
    mkdir $bundle.app/Contents/MacOS/
    mkdir $bundle.app/Contents/Resources/

    cp $exe $bundle.app/Contents/MacOS/$bundle
    cp assets/MacOS/Info.plist $bundle.app/Contents/Info.plist
    cp assets/MacOS/icon.icns $bundle.app/Contents/Resources/icon.icns
    cp -r lib $bundle.app/Contents/MacOS/lib/
    cp -r assets $bundle.app/Contents/Resources/assets/

    echo $bundle.app successfully created
}

init() {
    tar -xzvf ../Assets/lib.tar.gz -C .
    tar -xzvf ../Assets/assets.tar.gz -C .
    mkdir bin/
    pushd src/
    ./buildlibs.sh -d
    popd
}

clean() {
    rm -r bin
    rm -r lib
    rm -r assets
}

archive() {
    tar -czvf ../Assets/assets.tar.gz assets/
}

helpmsg() {
    echo "Use 'init' to initialize and compile sandbox."
    echo "Use 'clean' to remove all binaries and libraries built."
    echo "Use 'exe' to execute binary or program from /bin"
    echo "Use 'build_app' to build a MacOS App Bundle from your program"
}

if (($# >= 1))
then
    if [[ "$1" == "exe" ]]
    then
        shift
        exe "$@"
        exit
    elif [[ "$1" == "build_app" ]]
    then
        shift
        build_mac_app "$@"
        exit
    elif [[ "$1" == "init" ]]
    then
        init
        exit
    elif [[ "$1" == "clean" ]]
    then
        clean
        exit
    elif [[ "$1" == "archive" ]]
    then
        archive
        exit
    elif [[ "$1" == "help" ]]
    then
        helpmsg
        exit
    else
        echo "Invalid command. Use 'help' for more information."
        exit
    fi
fi

echo "Missing command. Use 'help' for more information."
