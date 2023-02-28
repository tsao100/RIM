rem clear build
del build\*.* /S /Q
rd build /S /Q
md build
cd build
echo type ..\qt-cmake to generate *.sln for Visual studio.