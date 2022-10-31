#! /bin/bash

login=$1

ESC="\033["
C_GREEN=$ESC"0;32m"
C_YELLOW=$ESC"0;33m"
C_BWHITE=$ESC"1;37m"
C_RST=$ESC"0m"

function line
{
    cols=$(tput cols)
    char=$1
    color=$2
    
    if test "$color" != ""; then
        echo -ne $color
    fi
    
    for i in $(eval echo "{1..$cols}"); do
        echo -n $char
    done
    echo
    
    if test "$color" != ""; then
        echo -ne $C_RST
    fi
}

echo -ne $C_GREEN
line "-"
echo "##> Rebuild - Cmake"
line "-"
echo -ne $C_RST
sudo cmake . -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
sudo cmake --build .