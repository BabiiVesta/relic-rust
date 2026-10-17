# RELIC & RUST — Core Game Concept

## Identity

Original single-player action RPG concept: a dense corporate megacity, a dangerous frontier, and buried civilizations whose machines can rewrite modern infrastructure. The first goal is a combat-focused vertical slice, not an open world. Touchstones are high-level genre references only; characters, lore, systems, assets, and implementation should remain original.

**Pillars:** (1) seamless hybrid combat, (2) exploration reveals usable technology, (3) faction choices change access and regional conditions. Third-person is the default for melee readability; first-person is an optional aiming/exploration camera after the combat slice works.

## Premise and factions

The player is a contract runner whose extraction team is destroyed when a recovered relic bonds to their nervous system. The relic interfaces with both commercial implants and ancient infrastructure. Its original purpose was to coordinate a civilization-wide network; the same network contributed to that civilization's collapse.

- **Meridian Combine:** wants licensed, predictable control of relic infrastructure.
- **Free Marches:** wants local ownership, while its settlements disagree on who should govern it.
- **Continuum:** wants the network sealed, even if that means abandoning useful technology.

The first slice ends at a decision: hand over a live relay map to one faction, destroy it, or release a limited copy to the settlement. This changes a later safehouse/mission lead; no choice is framed as purely good.

## Combat loop

Read threats and cover → overload a priority implant or device → close with blade or hold range with pistol → parry/dodge around telegraphs → exploit stagger/weak points → recover scrap and data that alter the next encounter. Attacks, shots, and hacks are separate actions sharing a small set of commitment states. Heavy actions expose the player; hacking builds neural heat. Start with one blade, one pistol, one melee enemy, one ranged enemy, and one overload hack.

## Prototype weapons and abilities

- **Arc blade:** quick three-hit string, committed heavy opener, directional dodge follow-up; electrical damage briefly disrupts powered armor.
- **Service pistol:** accurate single shots; exposed machinery can be shot to create a local hazard.
- **Overload:** short-range targeted interrupt, modest damage, adds heat; high heat blocks another cast until it dissipates.
- **Later hack families:** disruption (blackout/jam), control (spoof/hijack), reconnaissance (threat scan/trace), relic (phase step/system rewrite). Do not build these before the first hack is fun.

## Progression

Three flexible trees with cross-tree combinations, no locked classes:

- **Arsenal:** blade timing, firearm handling, armor and counterfire.
- **Interface:** hack range/heat, stealth, device control and chain effects.
- **Relic:** mobility, gravity effects and ancient machinery access.

Prototype progression is one upgrade choice after the encounter: faster heat recovery or stronger parry follow-up. Defer full skill UI and economy.

## World systems

Slice location: a collapsed transit station where a neon service district overlays a pre-human relay vault. Use one traversal loop, one optional cache, one fight arena, one relay interaction, and one decision. World systems to prototype later: faction alert/ownership, discoverable shortcuts powered by relic nodes, and a regional security state that changes patrol presence. Avoid simulation-heavy open-world systems until the slice validates movement and combat.

## First slice acceptance targets

One 10–15 minute mission. Player can move, switch camera, land blade hits, aim/fire, dodge, parry a telegraphed melee strike, overload a target, and complete a simple objective. Two enemy roles respond to at least two damage types. A single end-of-mission choice changes a visible follow-up flag. Stable 60 fps target on the chosen Mac hardware is a later profiling goal; first establish a reliable editor build.
