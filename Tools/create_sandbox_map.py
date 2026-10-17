import unreal

MAP_PATH = "/Game/RelicRust/Maps/L_CombatSandbox"

if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
else:
    if not unreal.EditorLevelLibrary.new_level(MAP_PATH):
        raise RuntimeError("Could not create the combat sandbox level")
    world = unreal.EditorLevelLibrary.get_editor_world()
if not world:
    raise RuntimeError("Could not load the combat sandbox level")

actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
level_editor = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
for actor in actors.get_all_level_actors():
    if isinstance(actor, unreal.PlayerStart):
        actors.destroy_actor(actor)
actors.spawn_actor_from_class(
    unreal.PlayerStart,
    unreal.Vector(0.0, 0.0, 118.0),
    unreal.Rotator(0.0, 0.0, 0.0),
)

if not any(isinstance(actor, unreal.DirectionalLight) for actor in actors.get_all_level_actors()):
    sun = actors.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 800.0), unreal.Rotator(-38.0, -32.0, 0.0))
    sun.get_component_by_class(unreal.DirectionalLightComponent).set_editor_property("intensity", 8.0)

game_mode = unreal.load_class(None, "/Script/RelicRust.RelicGameMode")
if game_mode:
    world.get_world_settings().set_editor_property("default_game_mode", game_mode)

if not level_editor.save_current_level():
    raise RuntimeError("Could not save the combat sandbox level")

unreal.log("Created and saved Relic & Rust combat sandbox: " + MAP_PATH)
