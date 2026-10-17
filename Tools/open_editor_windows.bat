@echo off
setlocal

set "PROJECT=%~1"
if not defined PROJECT for %%I in ("%~dp0..\RelicRust.uproject") do set "PROJECT=%%~fI"

if not defined UE_ROOT (
  echo Set UE_ROOT to your Unreal Engine installation directory, such as C:\Program Files\Epic Games\UE_5.8. 1>&2
  exit /b 2
)

set "EDITOR=%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe"
if not exist "%EDITOR%" (
  echo Unreal Editor not found: %EDITOR% 1>&2
  exit /b 2
)

if not exist "%PROJECT%" (
  echo Project file not found: %PROJECT% 1>&2
  exit /b 2
)

start "" "%EDITOR%" "%PROJECT%" -log
