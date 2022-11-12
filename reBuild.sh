#! /bin/bash

login=$1
defaultBuild="Release"
user_build_type="${@:2}" # get all arguments after the first one

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
if [ "$1" = "Re" ] ; then
    sudo rm -rf build cmake-build-release r_type_server r_type_client CmakeUserPresets.json
    sh ./install.sh "${user_build_type:-$defaultBuild}"
else
    sudo cmake --build build
fi

echo -ne $C_GREEN
line "-"
echo "##> Le script est fini"
line "-"
echo -ne $C_RST