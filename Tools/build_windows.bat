@echo off
setlocal

set "PROJECT=%~1"
if not defined PROJECT for %%I in ("%~dp0..\RelicRust.uproject") do set "PROJECT=%%~fI"

set "TARGET=%~2"
if not defined TARGET set "TARGET=RelicRustEditor"

set "CONFIGURATION=%~3"
if not defined CONFIGURATION set "CONFIGURATION=Development"

if not defined UE_ROOT (
  echo Set UE_ROOT to your Unreal Engine installation directory, such as C:\Program Files\Epic Games\UE_5.8. 1>&2
  exit /b 2
)

set "BUILD_SCRIPT=%UE_ROOT%\Engine\Build\BatchFiles\Build.bat"
if not exist "%BUILD_SCRIPT%" (
  echo Build script not found: %BUILD_SCRIPT% 1>&2
  exit /b 2
)

if not exist "%PROJECT%" (
  echo Project file not found: %PROJECT% 1>&2
  exit /b 2
)

call "%BUILD_SCRIPT%" "%TARGET%" Win64 "%CONFIGURATION%" "-project=%PROJECT%" -waitmutex
exit /b %ERRORLEVEL%
