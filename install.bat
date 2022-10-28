::TELECHARGE FICHIER
@ECHO OFF

::INSTALL CMAKE
"%~dp0\executable\cmake.msi"

::INSTALL CONAN
"%~dp0\executable\conan.exe"

::EXECUTE CONAN
conan install . -s compiler="Visual Studio" -s compiler.version=17 --install-folder cmake-build-release --build=missing -c tools.system.package_manager:mode=install
cmake . -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

cd Release
move r-type_client.exe "%~dp0"
move r-type_library.lib "%~dp0"
move r-type_server.exe "%~dp0"
cd ..