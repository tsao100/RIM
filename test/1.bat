@echo off

set PROJECT_NAME=my_project

rem Create folders
mkdir %PROJECT_NAME%
cd %PROJECT_NAME%
mkdir bin docs include lib src tests tools

rem Create files
cd docs
echo. > user_manual.pdf
cd ..

cd include
mkdir app
cd app
echo. > document.hpp
echo. > model.hpp
echo. > view.hpp
cd ..\..

cd lib
rem Create a placeholder file since we don't have any actual libraries
echo. > placeholder.lib
cd ..

cd src
mkdir app
cd app
echo. > document.cpp
echo. > model.cpp
echo. > view.cpp
cd ..\..
echo. > main.cpp
cd ..

cd tests
mkdir unit_tests integration_tests
cd unit_tests
echo. > test_document.cpp
echo. > test_model.cpp
echo. > test_view.cpp
cd ..\integration_tests
echo. > test_integration_1.cpp
echo. > test_integration_2.cpp
echo. > test_integration_3.cpp
cd ..\..
cd tools
mkdir build format
cd build
echo. > CMakeLists.txt
echo. > configure.sh
echo. > Makefile
cd ..\format
echo. > clang-format.sh
echo. > cpplint.py
cd ..\..

echo. > .gitignore
echo. > CMakeLists.txt
echo. > LICENSE
echo. > README.md

echo File and folder structure created successfully!
pause
