#! /bin/bash

defaultBuild="Release"
if [ -z "$2" ]
    then
        build=$defaultBuild
    else
        build=$2
fi

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
if [ $1 = "Re" ] ; then
    echo -ne $C_YELLOW
    line "-"
    echo "##> Rebuild from scratch - Cmake"
    echo "##> Forwarding arg : $build, as build type"
    line "-"
    echo -ne $C_RST
    sudo rm -rf build cmake-build-release r_type_server r_type_client CmakeUserPresets.json
    sh ./install.sh $build
else
    sudo cmake --build build
fi
sudo -k
