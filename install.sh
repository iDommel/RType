#! /bin/bash

# variables

## script

files="./.files"
dir_tmp="/tmp/install"

login=$1

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
	which zypper &> /dev/null && os="opensuse"
	which pacman &> /dev/null && os="archlinux"
	which dnf &> /dev/null && os="fedora"
	which apt-get &> /dev/null && os="debian"
	which emerge &> /dev/null && os="gentoo"
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
		opensuse)
			sudo zypper -y update
			;;
		archlinux)
			yay -S
			;;
		fedora)
			sudo dnf -y update
			;;
		debian)
			sudo apt -y update; sudo apt -y upgrade
			;;
		gentoo)
			sudo emerge -u world
			;;
	esac
}

function sys_install
{
	package_name=$1

	function get_cmd_install
	{
		case "$os" in
			opensuse)
				echo "zypper -y install"
				;;
			archlinux)
				echo "pacman --noconfirm -S"
				;;
			fedora)
				echo "dnf install -y"
				;;
			debian)
				echo "apt -y install"
				;;
			gentoo)
				echo "emerge"
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

script_init

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

script_header "TELECHARGE FICHIER"
git clone https://github.com/rox51124/File-R-Type.git; cd File-R-Type; mv conanfile.txt ../; cd ..; rm -rf File-R-Type

##which python marche pas
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
	echo "python not installed";
    sys_install pip python3-pip;
fi;

script_header "INSTALLATION DE CONAN"
if (which conan); then
    echo "conan already installed";
else
	echo "conan not installed";
    sudo pip install conan;
fi;

script_header "INSTALLATION DE PKG"
if (which pkg-config); then
    echo "pkg-config already installed";
else
	echo "pkg-config not installed";
    sys_install pkg-config;
fi;

script_header "LANCE CONAN"
conan install ./ --build=missing -c tools.system.package_manager:mode=install
rm -rf conan.lock conanbuildinfo.cmake conanbuildinfo.txt conanfile.txt conaninfo.txt graph_info.json

script_header "VOTRE INSTALLATION EST FINI" $C_BWHITE

sudo -k