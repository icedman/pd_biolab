project('game', 'c',
  version : '0.1',
  default_options : ['warning_level=3']
)

pwd = meson.current_source_dir()
cc = meson.get_compiler('c')

game_files = [
    'src/main.c'
]

engine_files = [
        'engine/platform.c',
        'engine/render.c',
        'engine/camera.c',
        'engine/utils.c',
        'engine/animation.c',
        'engine/image.c',
        'engine/input.c',
        'engine/trace.c',
        'engine/entity.c',
        'engine/engine.c',
        'engine/noise.c',
        'engine/font.c',
        'engine/map.c',
        'src/scenes/credits.c',
        'src/scenes/game.c',
        'src/scenes/stats.c',
        'src/scenes/title.c',
        'src/entities/blob.c',
        'src/entities/crate.c',
        'src/entities/debris.c',
        'src/entities/delay.c',
        'src/entities/dropper.c',
        'src/entities/earthquake.c',
        'src/entities/end_hub.c',
        'src/entities/end_hub_fade.c',
        'src/entities/end_hub_plasma.c',
        'src/entities/glass_dome.c',
        'src/entities/grunt.c',
        'src/entities/hurt.c',
        'src/entities/level_change.c',
        'src/entities/mine.c',
        'src/entities/mover.c',
        'src/entities/particle.c',
        'src/entities/player.c',
        'src/entities/projectile.c',
        'src/entities/respawn_pod.c',
        'src/entities/spewer.c',
        'src/entities/spewer_shot.c',
        'src/entities/spike.c',
        'src/entities/test_tube.c',
        'src/entities/trigger.c',
        'src/entities/void.c',
        'src/main.c',
]

includes = include_directories(
    'engine',
    'libs',
    'src',
)

m_dep = cc.find_library('m', required : true, disabler: true)
gl_dep = dependency('opengl', required: true, disabler: true)
sdl2_dep = dependency('SDL2', required: true, disabler: true)
sdl2_mixer_dep = dependency('SDL2_Mixer', required: true, disabler: true)
sdl2_image_dep = dependency('SDL2_image', required: true, disabler: true)

executable('game',
    engine_files,
    game_files,
    include_directories: [ includes ],
    c_args: ['-DRENDER_SOFTWARE', '-DPLATFORM_SDL'],
    dependencies: [ m_dep, sdl2_dep, sdl2_mixer_dep, sdl2_image_dep, gl_dep ]
)
