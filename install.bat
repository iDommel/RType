@ECHO OFF

::INSTALL CMAKE
::"%~dp0\executable\cmake.msi"

::INSTALL CONAN
::"%~dp0\executable\conan.exe"

::EXECUTE CONAN
conan install . -s compiler="Visual Studio" -s compiler.version=17 --install-folder cmake-build-release --build=missing -c tools.system.package_manager:mode=install
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release