#! /bin/bash

# variables

## script

files="./.files"
dir_tmp="/tmp/install"

defaultBuild="Release"
if [ -z "$1" ]
    then
        build=$defaultBuild
    else
        build=$1
fi

## fake install ?
fake=0

## colors

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

function script_header
{
    color=$2
    if test "$color" = ""; then
        color=$C_RED
    fi

    echo -ne $color
    line "-"
    echo "##> "$1
    line "-"
    echo -ne $C_RST
}

function get_os_type
{
    which dnf &> /dev/null && os="fedora"
    which apt-get &> /dev/null && os="ubuntu"
}

function script_init
{
    os="void"
    get_os_type

    if test "$os" = "void"; then
        line "#" $C_YELLOW
        script_header "VOTRE DISTRIBUTION N'EST PAS SUPPORTÉE..."
        line "#" $C_YELLOW
        exit 42
    fi

    rm -rf $dir_tmp
    mkdir $dir_tmp
}

function sys_upgrade
{
    if test $fake -eq 1; then
        echo "Upgrade system"
        return
    fi
    case "$os" in
        fedora)
            sudo dnf -y update
        ;;
        ubuntu)
            sudo apt -y update
        ;;
    esac
}

function sys_install
{
    package_name=$1

    function get_cmd_install
    {
        case "$os" in
            fedora)
                echo "dnf install -y"
            ;;
            ubuntu)
                echo "apt -y install"
            ;;
        esac
    }

    if test -z "$cmd_install"; then
        cmd_install=$(get_cmd_install)
    fi

    if test $fake -eq 1; then
        echo "Installing" $package_name "(command:" $cmd_install $package_name ")"
        return
    fi

    sudo $cmd_install $package_name
}

function script_install
{
    if test $fake -eq 1; then
        echo "Installing" $1 "(script_install)"
        return
    fi
    sudo cp $files/$1 /usr/bin/$1
    sudo chmod 755 /usr/bin/$1
}

echo -ne $C_YELLOW
line "-"
echo "##> Using $build as build type"
line "-"
echo -ne $C_RST

script_init

script_header "MISE À JOUR DES PAQUETS DU SYSTÈME"
sys_upgrade

script_header "INSTALLATION DE GCC"
if (which gcc); then
    echo "gcc already installed";
else
    echo "gcc not installed";
    sys_install gcc;
fi;

script_header "INSTALLATION DE G++"
if (which g++); then
    echo "g++ already installed";
else
    echo "g++ not installed";
    sys_install g++;
fi;

script_header "INSTALLATION DE CMAKE"
if (which cmake); then
    echo "cmake already installed";
else
    echo "cmake not installed";
    sys_install cmake;
fi;

script_header "INSTALLATION DE GIT"
if (which git); then
    echo "git already installed";
else
    echo "git not installed";
    sys_install git;
fi;

script_header "INSTALLATION DE WHICH"
sys_install which;

script_header "INSTALLATION DE PYTHON"
if (which python3); then
    echo "python already installed";
else
    echo "python not installed";
    sys_install python3;
fi;

script_header "INSTALLATION DE PIP"
if (which pip); then
    echo "pip already installed";
else
    echo "pip not installed";
    sys_install pip;
    sys_install python3-pip;
fi;

script_header "INSTALLATION DE CONAN"
if (which conan); then
    echo "conan already installed";
else
    echo "conan not installed";
    sudo pip install conan;
fi;

script_header "INSTALLATION DE PERL-FINDBIN"
# unfortunately which doesn't work for this package
if (find /usr/local -name FindBin.pm | wc -l != 0);
then
    echo "perl findbin already installed";
else
    echo "perl findbin not installed";
    sys_install perl-FindBin;
fi;

script_header "INSTALLATION DE PERL"
if (which perl); then
    echo "perl already installed";
else
    echo "perl not installed";
    sys_install perl;
fi;

script_header "INSTALLATION DE PKG"
if (which pkg-config); then
    echo "pkg-config already installed";
else
    echo "pkg-config not installed";
    sys_install pkg-config;
fi;

script_header "LANCE CONAN"
export CONAN_SYSREQUIRES_MODE=enabled
sudo conan install . --install-folder cmake-build-release --build=missing -c tools.system.package_manager:mode=install

script_header "CMAKE Build"
sudo cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$build
sudo cmake --build build

script_header "VOTRE INSTALLATION EST FINI" $C_BWHITE

sudo -k
