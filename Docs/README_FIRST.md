# Relic & Rust — UE 5.8 prototype

This folder is a standalone C++ Unreal project. It builds a runtime-generated greybox arena and a playable combat loop without paid assets or plugins.

## Open and build on Windows

1. Install Unreal Engine 5.8 and a supported Visual Studio C++ toolchain. For UE 5.8, Epic lists Visual Studio 2026 as recommended and Visual Studio 2022 17.14 or later as supported. Install the C++ and Game development with C++ workloads plus a Windows SDK. See [Epic's Visual Studio setup guide](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine).
2. Clone the repository, then double-click `RelicRust.uproject`. If Unreal asks to build missing modules, allow it. The project opens on Unreal's built-in Entry map; the prototype arena is spawned by the game mode when you press Play.
3. To build from Command Prompt, set `UE_ROOT` to the Unreal Engine folder and run:

   ```bat
   set "UE_ROOT=C:\Program Files\Epic Games\UE_5.8"
   Tools\build_windows.bat
   ```

   The script defaults to `RelicRustEditor Win64 Development`. Pass `RelicRust.uproject` as the first argument, a target such as `RelicRust` as the second, and a configuration such as `Development` as the third.
4. Press **Play**. The game mode creates the blockout arena and enemies at runtime; no authored map or marketplace asset is required.

## Open and build on macOS

Install Unreal Engine 5.8 and Xcode command-line tools, then double-click `RelicRust.uproject`. To build the editor target from Terminal:

```sh
UE_ROOT="/path/to/UnrealEngine" ./Tools/build_mac.sh "$PWD/RelicRust.uproject" RelicRustEditor
```

## Controls

| Action | Keyboard/mouse |
|---|---|
| Move / look | WASD / mouse |
| Jump | Space |
| Blade attack | Left mouse |
| Heavy blade attack | R |
| Pistol | Right mouse |
| Dodge | Left Shift |
| Parry | Hold Q as a melee enemy attacks |
| Overload | E while aiming at an enemy |
| Switch first/third person | V |
| Interact / choose ending | F / 1, 2, or 3 |

The configured gamepad layout uses the left stick to move, left-stick click to jump, right stick to look, bottom face button to interact, left face button for a light attack, top face button for heavy attack, right face button to dodge, left shoulder to parry, right trigger to fire, and left trigger to overload. Keyboard and mouse remain the primary, documented layout.

The player carries a multi-part relic blade and a compact pistol made from bundled UE primitive meshes with metal and orange-accent materials. In first-person, the weapon viewmodel switches to the blade during light/heavy attacks and to the pistol when firing. In third-person, both remain visible on the character.

The overlay displays health, neural heat, combat state, the current objective and controls. Read the field record, defeat both defenders, reach the relic relay, then choose who receives the route (or erase it). The rifleman attacks from range while the raider closes in; each shows an attack warning before firing or striking. A successful parry counters for damage. Chain three timed light attacks for a stronger finisher. The selected ending is also retained as a game-mode flag for later mission logic. Player death currently resets health and teleports the player to the arena start.

## What is implemented

- C++ player character with movement, jump, third-person spring-arm camera and first-person camera toggle.
- Primitive blade and pistol blockout meshes, three-step sword combo with stronger finisher, committed heavy sphere sweep, pistol view trace, dodge impulse, timed parry window/counter, overload target trace and passive neural-heat recovery.
- Melee chaser and ranged rifleman with health, damage, simple pursuit/spacing, attacks and damage labels. The rifleman backs away if the player closes to melee range.
- Runtime-generated blockout arena and basic HUD.
- A short mission sequence with a field record, combat gate, relay interaction and three ending choices.
- Game and editor build targets for UE 5.8 on Mac and Windows.

## Prototype limits

This is a code-first greybox, not a polished vertical slice. Weapons and characters are still primitive blockout models; the weapons have no authored animation, recoil, or audio. The current AI uses direct movement and line traces; environment collision can affect it. See `BUILD_NOTES.md` for current build status and known limitations.

## Sharing and version control

The project is shared in the Relic & Rust GitHub repository. Keep `Config`, `Docs`, `Source`, `Tools`, and the `.uproject`; omit `Binaries`, `DerivedDataCache`, `Intermediate`, `Saved`, generated IDE files, and build logs when sharing. Use Git LFS if you later add large binary assets.
