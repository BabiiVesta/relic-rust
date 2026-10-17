# UE5 Roadmap — Mac-first, automation-heavy

## Current state (2026-10-16)

The UE 5.8 C++ project scaffold and a code-first combat/mission greybox are in place. The editor target has compiled on the target Mac. The Mac game target has not yet produced a verified, standalone, cooked app, and the loop still needs an interactive playtest. Next: validate Play in Editor, tune combat feel, author a readable enemy telegraph, then create a saved transit-station blockout. The phases below describe work beyond this prototype pass.

## Phase 0 — Establish a reproducible project (1–2 sessions)

Completed for this pass: create the C++ project and confirm an editor-target build. The project uses legacy input mappings and engine primitives, with no paid plugins or marketplace assets. Remaining: validate the packaged Development build and continue source control before broadening the content.

## Phase 1 — Combat sandbox (2–4 sessions)

Implemented as a first pass: runtime greybox arena, keyboard/gamepad bindings, HUD, health, three-step light combo, committed heavy, pistol, dodge, parry, overload, and two damage-responsive enemy roles. Remaining: interactive validation, animation-driven hit windows, and short capture evidence.

## Phase 2 — Hybrid encounter (3–6 sessions)

Implemented as a first pass: ranged rifleman, relay/record interactions, camera toggle, and neural heat display. Remaining: aim assist/target selection, weapon switching, environmental hazard, authored AI states, and more reliable combat chaining.

## Phase 3 — Mission slice (1–3 weeks after sandbox)

Implemented as a compact runtime arena: field record, two-enemy encounter, relay interaction, three ending choices, visible ending text, and a Blueprint-readable ending flag (1–3). Remaining: author a transit-station map with a traversal route and optional cache, use the ending flag for a follow-up safehouse lead, and validate a full short mission. Later systems: faction ownership, relic shortcuts, and regional security state.

## Phase 4 — Polish and share

Next: interactively profile on the target Mac; improve combat feel; produce and test a clean Development package from a fresh folder; then share through private Git or AirDrop. The current source ZIP should exclude generated build products and logs.

## Mac constraints and cost

The prototype was built with Unreal Engine 5.8, Xcode 27.0, and an Apple Clang arm64 toolchain; see `Docs/BUILD_NOTES.md`. No paid assets or plugins are required. Verify any future marketplace content and plugins for macOS support before adoption.

## AI handoff prompt

“Work only in the RelicRust project. Inspect the existing module name and UE version before editing. Make the smallest change needed for [feature]. Keep gameplay actions independent and preserve the current public component API. Explain each changed file, include exact Mac build steps, and identify anything that must be set in the editor. Do not add plugins, paid assets, or broad refactors. Stop if compilation fails and report the first actionable error.”
