@echo off
echo Cleaning R-Type project...

:: Delete executables
echo Removing executables...
if exist r-type_client.exe del /f /q r-type_client.exe
if exist r-type_server.exe del /f /q r-type_server.exe

:: Delete PDB files
echo Removing PDB files...
del /f /q *.pdb

:: Delete CMake generated files
echo Removing CMake cache files...
if exist CMakeCache.txt del /f /q CMakeCache.txt
if exist cmake_install.cmake del /f /q cmake_install.cmake
if exist CPackConfig.cmake del /f /q CPackConfig.cmake
if exist CPackSourceConfig.cmake del /f /q CPackSourceConfig.cmake

:: Delete build folder
echo Removing build directory...
if exist build rmdir /s /q build

:: Delete other generated files
echo Removing other build artifacts...
if exist r-type_library.lib del /f /q r-type_library.lib
if exist r-type_library.dll del /f /q r-type_library.dll
if exist compile_commands.json del /f /q compile_commands.json

echo Cleanup complete!
pause
