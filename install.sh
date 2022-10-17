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
C_RED=$ESC"0;31m"
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
				echo "dnf -y install"
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

script_header "INSTALLATION DE CONAN PIP"
if (which conan); then
    echo "conan already installed";
else
	echo "pip not installed";
    pip install conan;
fi;

script_header "INSTALLATION PACKET"

##cat /etc/os-release | grep "fedora"
##if [[ $? -ne 0 ]]; then
##    echo "bad os";
##else
##	sudo dnf group install "Development Tools"
##	sudo dnf group install "Development Libraries"
##	sys_install libglvnd-devel
##fi

if (which perl); then
    echo "perl already installed";
else
	echo "perl not installed";
    sys_install perl;
fi;

if (which openssl); then
    echo "openssl already installed";
else
	echo "openssl not installed";
    sys_install openssl;
fi;

if (which pkg-config); then
   echo "pkg-config already installed";
else
	echo "pkg-config not installed";
    sys_install pkg-config;
fi

##Si fedora :
cat /etc/os-release | grep "fedora"
if [[ $? -ne 0 ]]; then
    echo "bad os";
else
	sudo dnf install libglvnd-devel
  	sudo dnf install libfontenc-devel
	sudo dnf install libXaw-devel
	sudo dnf install libXcomposite-devel
	sudo dnf install libXcursor-devel
	sudo dnf install libXdmcp-devel
	sudo dnf install libXtst-devel
	sudo dnf install libXinerama-devel
	sudo dnf install libxkbfile-devel
	sudo dnf install libXrandr-devel
	sudo dnf install libXres-devel
	sudo dnf install libXScrnSaver-devel
	sudo dnf install libXvMC-devel
	sudo dnf install xorg-x11-xtrans-devel
	sudo dnf install xcb-util-wm-devel
	sudo dnf install xcb-util-image-devel
	sudo dnf install xcb-util-keysyms-devel
	sudo dnf install xcb-util-renderutil-devel
	sudo dnf install libXdamage-devel
	sudo dnf install libXxf86vm-devel
	sudo dnf install libXv-devel
	sudo dnf install xcb-util-devel
	sudo dnf install libuuid-devel
	sudo dnf install xkeyboard-config-devel;
fi

##Si ubuntu :
cat /etc/os-release | grep "ubuntu"
if [[ $? -ne 0 ]]; then
    echo "bad os";
else
	sys_install build-essential
    sys_install libxcb-util-dev
    sys_install libgl-dev
    sys_install libx11-xcb-dev
    sys_install libfontenc-dev
    sys_install libice-dev
    sys_install libsm-dev
    sys_install libxaw7-dev
    sys_install libxcomposite-dev
    sys_install libxcursor-dev
    sys_install libxdamage-dev
    sys_install libxext-dev
    sys_install libxfixes-dev
    sys_install libxi-dev
    sys_install libxinerama-dev
    sys_install libxkbfile-dev
    sys_install libxmu-dev
    sys_install libxmuu-dev
    sys_install libxpm-dev
    sys_install libxrandr-dev
    sys_install libxrender-dev
    sys_install libxres-dev
    sys_install libxss-dev
    sys_install libxt-dev
    sys_install libxtst-dev
    sys_install libxv-dev
    sys_install libxvmc-dev
    sys_install libxxf86vm-dev
    sys_install libxcb-render0-dev
    sys_install libxcb-render-util0-dev
    sys_install libxcb-xkb-dev
    sys_install libxcb-icccm4-dev
    sys_install libxcb-image0-dev
    sys_install libxcb-keysyms1-dev
    sys_install libxcb-randr0-dev
    sys_install libxcb-shape0-dev
    sys_install libxcb-sync-dev
    sys_install libxcb-xfixes0-dev
    sys_install libxcb-xinerama0-dev
    sys_install libxcb-dri3-dev
    sys_install uuid-dev;
fi

script_header "LANCE CONAN"
conan install conanfile.txt --build=qt -c tools.system.package_manager:mode=install --build=missing

script_header "VOTRE INSTALLATION EST FINI" $C_BWHITE

sudo -k