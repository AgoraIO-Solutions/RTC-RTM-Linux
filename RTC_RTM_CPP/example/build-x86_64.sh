#!/bin/bash

cd $(dirname $(readlink -f $0)); CURRENT=$(pwd); cd -
MACH=$(echo $(basename ${0%.*}) | awk -F - '{print $2}')

target_os="linux"
license="n"

build_op="build"
build_type="release"

test -z "${android_abi}" && android_abi="arm64-v8a"
test -z "${android_api}" && android_api=24

toolchain=$CURRENT/scripts/toolchain.cmake
test -z "$ANDROID_NDK" && ANDROID_NDK=${ANDROID_NDK_ROOT}

# to clean
build_files=("build")

function cmake_build() {
    src=$(readlink -f $1)
    shift
    if [ -d build ]; then
        rm -rf build
    fi

    if [ ${build_type} == "debug" ]; then
        debug_flag="-DCMAKE_BUILD_TYPE=Debug "
    elif [ ${build_type} = "asan" ]; then
        debug_flag="-DCMAKE_BUILD_TYPE=asan "
    fi

    if [ ${target_os} == "linux" ]; then
        echo "target OS is Linux"
        os_flag="-DCMAKE_TOOLCHAIN_FILE=$toolchain"
    elif [ ${target_os} = "android" ]; then
        echo "target OS is Android"
        os_flag="-DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=${android_abi} -DANDROID_NATIVE_API_LEVEL=${android_api}"
    fi

    mkdir build && cd build &&
        cmake $src $debug_flag $os_flag -Wno-dev -DLICENSE_CHECK="${license}" -DMACHINE=$MACH $* &&
        make -j8 && sudo ldconfig || return 1
    cd -
}

function clean_build_files() {
    for file in ${build_files[@]}; do
        if [ -d ${file} ]; then
            rm -rf ${file}
        elif [ -f ${file} ]; then
            rm ${file}
        fi
    done

}

function help() {
    echo -e "$0 [-b <build_op>] [-l <license>] [-t <type>] [-o <os>] [-f <toolchain>]"
    echo -e "\t -b <build_op>, build|clean|rebuild, default: $build_op"
    echo -e "\t -l <license>, y|n, default: $license"
    echo -e "\t -o <os>, linux|android, default: $target_os"
    echo -e "\t -f <toolchain>, default: $toolchain"
    echo -e "\t -t <type>, release|debug|asan, default: $build_type"
}

while getopts 'b:l:o:t:f:h' opt; do
    case $opt in
    b)
        build_op=${OPTARG}
        ;;
    l)
        license=${OPTARG}
        ;;
    o)
        target_os=${OPTARG}
        ;;
    t)
        build_type=${OPTARG}
        ;;
    f)
        toolchain=${OPTARG}
        ;;
    h)
        help
        exit 1
        ;;
    \?)
        help
        exit 1
        ;;
    esac
done

if [ "${build_op}" != "build" -a "${build_op}" != "rebuild" -a "${build_op}" != "clean" ]; then
    echo "error build_op: "${build_op}""
    exit 1
fi

if [ "${license}" != "y" -a "${license}" != "n" ]; then
    echo "error license: "${license}""
    exit 1
fi

if [ "${build_op}" = "build" ]; then
    echo "building ..."
    cmake_build $CURRENT || exit 1
    echo "build done"
elif [ "${build_op}" = "clean" ]; then
    echo "cleaning ..."
    clean_build_files || exit 1
    echo "clean done"
elif [ "${build_op}" = "rebuild" ]; then
    echo "rebuilding ..."
    clean_build_files || exit 1
    cmake_build $CURRENT || exit 1
    echo "rebuild done"
fi
