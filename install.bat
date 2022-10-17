::TELECHARGE FICHIER
@ECHO OFF
git clone https://github.com/rox51124/File-R-Type.git
cd File-R-Type
move conanfile.txt "%~dp0"
cd ..
RMDIR /s /q "File-R-Type"

::INSTALL CMAKE
"%~dp0\Executable\cmake.msi"

::INSTALL CONAN
"%~dp0\Executable\conan.exe"

::EXECUTE CONAN
conan install conanfile.txt --build=qt -c tools.system.package_manager:mode=install --build=missing