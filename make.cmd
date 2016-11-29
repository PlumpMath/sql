@echo off
pushd %~dp0

rem Project
for /f "tokens=2 delims=( " %%i in ('findstr /c:"project(" CMakeLists.txt') do (
  set project=%%i
)

rem Settings
set generator=Visual Studio 14 2015 Win64
set toolset=v140
set configs=Release;Debug

if exist ..\settings.cmd (
  call ..\settings.cmd
)

echo.
echo Project        : "%project%"
echo Generator      : "%generator%"
echo Toolset        : "%toolset%"
echo Configurations : "%configs%"
echo.

rem Build Directory
if not exist build (
  mkdir build
)
pushd build

rem Solution
cmake -G "%generator%" -T "%toolset%" -DCMAKE_CONFIGURATION_TYPES="%configs%" -DCMAKE_INSTALL_PREFIX:PATH=.. ..
if %errorlevel% == 0 (
  for %%a in (%configs%) do (
    cmake --build . --target install --config "%%a"
  )
) else (
  pause
)

popd
popd
