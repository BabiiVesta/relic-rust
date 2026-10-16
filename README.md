# Relic & Rust

An Unreal Engine 5.8 C++ cyberpunk action prototype. It features a runtime-built greybox arena, melee and pistol combat, two enemy types, and a short mission with branching endings. The sword and pistol are placeholder blockout meshes.

## Get started

1. Install Unreal Engine 5.8 and Xcode command-line tools on macOS.
2. Open `RelicRust.uproject` in Unreal Engine. Allow the editor to compile the project modules if prompted.
3. Open the built-in Entry map, then press **Play**. The arena and combat encounter are created at runtime.

For build instructions and the full controls list, see [Docs/README_FIRST.md](Docs/README_FIRST.md). For current status and known limitations, see [Docs/BUILD_NOTES.md](Docs/BUILD_NOTES.md) and [Docs/ROADMAP.md](Docs/ROADMAP.md).

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

## Project contents

The repository contains the Unreal project, source code, configuration, build scripts, and documentation. Generated engine output (`Binaries`, `DerivedDataCache`, `Intermediate`, and `Saved`) is excluded. Unreal Engine 5.8 is required to build and run the game.
