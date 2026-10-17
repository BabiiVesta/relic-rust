# Relic & Rust

An Unreal Engine 5.8 C++ cyberpunk action prototype. It features a runtime-built greybox arena, melee and pistol combat, two enemy types, and a short mission with branching endings. The sword and pistol are placeholder blockout meshes.

## Get started on Windows

1. Install Unreal Engine 5.8 and a supported Visual Studio toolchain. Epic lists Visual Studio 2026 as recommended and Visual Studio 2022 17.14 or newer as supported for UE 5.8; enable the C++ and Game development with C++ workloads and a Windows SDK. See [Epic's Visual Studio setup guide](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine).
2. Clone this repository, then open `RelicRust.uproject` in Unreal Engine. If prompted to build missing modules, allow it.
3. To build from Command Prompt, set `UE_ROOT` to your Unreal Engine 5.8 install folder and run `Tools\build_windows.bat`. This builds the editor target by default. To build the game target instead, run `Tools\build_windows.bat "%CD%\RelicRust.uproject" RelicRust`.
4. Open the built-in Entry map and press **Play**. The arena and combat encounter are created at runtime.

## Get started on macOS

Install Unreal Engine 5.8 and Xcode command-line tools, open `RelicRust.uproject`, and allow the editor to compile missing modules if prompted. To build the editor target from Terminal:

```sh
UE_ROOT="/path/to/UnrealEngine" ./Tools/build_mac.sh "$PWD/RelicRust.uproject" RelicRustEditor
```

## Controls

| Action | Control |
|---|---|
| Move / look | WASD / mouse |
| Jump | Space |
| Light / heavy sword attack | Left mouse / R |
| Switch to pistol / fire | Right mouse |
| Dodge / parry / overload | Left Shift / hold Q / E |
| Switch first- and third-person camera | V |
| Interact / choose ending | F / 1, 2, or 3 |

For the full controls, setup steps, and prototype limitations, see [Docs/README_FIRST.md](Docs/README_FIRST.md), [Docs/BUILD_NOTES.md](Docs/BUILD_NOTES.md), and [Docs/ROADMAP.md](Docs/ROADMAP.md).

## Project contents

The repository contains the Unreal project, source code, configuration, build scripts, and documentation. Generated engine output (`Binaries`, `DerivedDataCache`, `Intermediate`, `Saved`, Visual Studio solution files, and IDE state) is excluded. Unreal Engine 5.8 is required to build and run the game. Use GitHub collaborators or pull requests to share edits; don't commit generated engine folders.
