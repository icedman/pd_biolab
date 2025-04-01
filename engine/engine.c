#include "engine.h"
#include "entity.h"
#include "input.h"
#include "platform.h"
#include "render.h"
// #include "alloc.h"
#include "utils.h"
// #include "image.h"
// #include "sound.h"

#ifdef PLATFORM_PLAYDATE
#include "pd_api.h"
extern PlaydateAPI *playdate;
#endif

engine_t engine = {.time_real = 0,
                   .time_scale = 1.0,
                   .time = 0,
                   .tick = 0,
                   .frame = 0,
                   .collision_map = NULL,
                   .gravity = 1.0,
                   .background_maps_len = 0};

static scene_t *scene = NULL;
static scene_t *scene_next = NULL;

// static texture_mark_t init_textures_mark;
// static image_mark_t init_images_mark;
// static bump_mark_t init_bump_mark;
// static sound_mark_t init_sounds_mark;

static bool is_running = false;

extern void main_init(void);
extern void main_cleanup(void);

void engine_init(void) {

  engine.time_real = platform_now();
  render_init(platform_screen_size());
  // sound_init(platform_samplerate());
  // platform_set_audio_mix_cb(sound_mix_stereo);
  input_init();
  entities_init();
  main_init();

  // init_bump_mark = bump_mark();
  // init_images_mark = images_mark();
  // init_sounds_mark = sound_mark();
  // init_textures_mark = textures_mark();
}

void engine_cleanup(void) {
  entities_cleanup();
  main_cleanup();
  input_cleanup();
  // sound_cleanup();
  render_cleanup();
}

#if 0 // PLATFORM_PLAYDATE

static entity_t null_entity;
static entity_t *current_entity = NULL;
static char last_entity_path[128] = "";
static map_t *current_map = NULL;
static char last_map_path[128] = "";
static int current_map_data_index = 0;

void decodeError(json_decoder *decoder, const char *error, int linenum) {}

void willDecodeSublist(json_decoder *decoder, const char *name,
                       json_value_type type) {
  // printf("%s {\n", decoder->path);
}

int shouldDecodeTableValueForKey(json_decoder *decoder, const char *key) {
  if (strstr(decoder->path, "entities[") == decoder->path) {
    int len = strlen(decoder->path);
    if (strstr(last_entity_path, decoder->path) == last_entity_path ||
        decoder->path[len - 1] != ']') {
      // same entity
    } else {
      // printf("new entity %s\n", decoder->path);
      strcpy(last_entity_path, decoder->path);
      current_entity = &null_entity;
    }
  }
  if (strstr(decoder->path, "maps[") == decoder->path) {
    int len = strlen(decoder->path);
    if (strstr(last_map_path, decoder->path) == last_map_path ||
        decoder->path[len - 1] != ']') {
      // same entity
    } else {
      // printf("new map %s\n", decoder->path);
      strcpy(last_map_path, decoder->path);
      current_map = malloc(sizeof(map_t));
      memset(current_map, 0, sizeof(map_t));
    }
  }
  return 1;
}

void *didDecodeSublist(json_decoder *decoder, const char *name,
                       json_value_type type) {
  return 0;
}

void didDecodeTableValue(json_decoder *decoder, const char *key,
                         json_value value) {
  char stringval[64] = "";
  int intval = 0;
  float floatval = 0;
  bool boolval = false;

  switch (value.type) {
  case kJSONString:
    strcpy(stringval, value.data.stringval);
    // printf("%s = %s\n", key, value.data.stringval);
    break;
  case kJSONInteger:
    intval = value.data.intval;
    floatval = intval;
    // printf("%s %s = %d\n", decoder->path, key, value.data.intval);
    break;
  case kJSONFloat:
    floatval = value.data.floatval;
    intval = floatval;
    // printf("%s %s = %f\n", decoder->path, key, value.data.floatval);
    break;
  case kJSONTrue:
    boolval = true;
    // printf("%s %s = True\n", decoder->path, key);
    break;
  case kJSONFalse:
    boolval = false;
    // printf("%s %s = False\n", decoder->path, key);
    break;
  default:
    break;
  }

  if (strstr(decoder->path, "entities[") == decoder->path) {
    if (strstr(decoder->path, "settings") == decoder->path) {
      if (strcmp("name", key) == 0) {
        char *name = value.data.stringval;
      }
    } else {
      if (strcmp("type", key) == 0) {
        entity_type_t entity_type = entity_type_by_name(stringval);
        current_entity = entity_spawn(entity_type, vec2(0, 0));
      }
      if (strcmp("x", key) == 0) {
        current_entity->pos.x = intval;
      }
      if (strcmp("y", key) == 0) {
        current_entity->pos.y = intval;
      }
    }
  }

  if (strstr(decoder->path, "maps[") == decoder->path) {
    map_t *map = current_map;
    bool size = false;
    if (strcmp("width", key) == 0) {
      map->size.x = intval;
      size = true;
    }
    if (strcmp("height", key) == 0) {
      map->size.y = intval;
      size = true;
    }
    if (strcmp("tilesize", key) == 0) {
      map->tile_size = intval;
    }
    if (strcmp("distance", key) == 0) {
      map->distance = intval;
    }
    if (strcmp("foreground", key) == 0) {
      map->foreground = boolval;
    }
    if (strcmp("repeat", key) == 0) {
      map->repeat = boolval;
    }
    if (strcmp("name", key) == 0) {
      char *name = value.data.stringval;
      if (str_equals(name, "collision")) {
        engine_set_collision_map(map);
      } else {
        engine_add_background_map(map);
      }
    }
    if (strcmp("tilesetName", key) == 0) {
      char *tileset_name = value.data.stringval;
      if (tileset_name && tileset_name[0]) {
        map->tileset = image(tileset_name);
      }
    }

    if (size && (map->size.x * map->size.y) > 0) {
      current_map_data_index = 0;
      int dataSize = sizeof(uint16_t) * map->size.x * map->size.y;
      map->data = malloc(dataSize);
      memset(map->data, 0, dataSize);
      // printf("%d %d %d\n", map->size.x, map->size.y, dataSize);
    }
  }
}

int shouldDecodeArrayValueAtIndex(json_decoder *decoder, int pos) { return 1; }

void didDecodeArrayValue(json_decoder *decoder, int pos,
                         json_value value) { // if pos==0, this was a bare value
                                             // at the root of the file
  int len = strlen(decoder->path);
  if (decoder->path[len - 1] == ']') {
    current_map->data[current_map_data_index++] = value.data.intval;
  }
}

void engine_load_level(char *json_path) {
  entities_reset();
  engine.background_maps_len = 0;
  engine.collision_map = NULL;

  current_entity = NULL;
  strcpy(last_entity_path, "");
  current_map = NULL;
  strcpy(last_map_path, "");

  json_decoder decoder = {
      .decodeError = decodeError,
      .willDecodeSublist = willDecodeSublist,
      .didDecodeSublist = didDecodeSublist,
      .shouldDecodeTableValueForKey = shouldDecodeTableValueForKey,
      .didDecodeTableValue = didDecodeTableValue,
      .shouldDecodeArrayValueAtIndex = shouldDecodeArrayValueAtIndex,
      .didDecodeArrayValue = didDecodeArrayValue};

  uint32_t len;
  uint8_t *data = platform_load_asset(json_path, &len);
  json_value value;
  playdate->json->decodeString(&decoder, (void *)data, &value);
  free(data);
}
#else

void engine_load_level(char *json_path) {
  json_t *json = platform_load_asset_json(json_path);
  error_if(!json, "Could not load level json at %s", json_path);

  entities_reset();
  engine.background_maps_len = 0;
  engine.collision_map = NULL;

  json_t *maps = json_value_for_key(json, "maps");
  for (int i = 0; maps && i < maps->len; i++) {
    json_t *map_def = json_value_at(maps, i);
    char *name = json_string(json_value_for_key(map_def, "name"));
    map_t *map = map_from_json(map_def);

    if (str_equals(name, "collision")) {
      engine_set_collision_map(map);
    } else {
      engine_add_background_map(map);
    }
  }

  json_t *entities = json_value_for_key(json, "entities");

  // Remember all entities with settings; we want to apply these settings
  // only after all entities have been spawned.
  // FIXME: we do this on the stack. Should maybe use the temp alloc instead.
  struct {
    entity_t *entity;
    json_t *settings;
  } entity_settings[entities->len];
  int entity_settings_len = 0;

  for (int i = 0; entities && i < entities->len; i++) {
    json_t *def = json_value_at(entities, i);

    char *type_name = json_string(json_value_for_key(def, "type"));
    error_if(!type_name, "Entity has no type");

    entity_type_t type = entity_type_by_name(type_name);
    error_if(!type, "Unknown entity type %s", type_name);

    vec2_t pos = {json_number(json_value_for_key(def, "x")),
                  json_number(json_value_for_key(def, "y"))};

    entity_t *ent = entity_spawn(type, pos);
    json_t *settings = json_value_for_key(def, "settings");
    if (ent && settings && settings->type == JSON_OBJECT) {

      // Copy name, if we have one
      json_t *name = json_value_for_key(settings, "name");
      if (name && name->type == JSON_STRING) {
        // TODO! free somewhere
        // freed at entities_reset
        ent->name = malloc(name->len + 1);
        strcpy(ent->name, name->string);
      }

      entity_settings[entity_settings_len].entity = ent;
      entity_settings[entity_settings_len].settings = settings;
      entity_settings_len++;
    }
  }

  for (int i = 0; i < entity_settings_len; i++) {
    // entity_settings(entity_settings[i].entity, entity_settings[i].settings);
  }
  free(json);
}
#endif

void engine_maps_reset() {
  for (int i = 0; i < engine.background_maps_len; i++) {
    map_t *map = engine.background_maps[i];
    free(map->data);
    free(map);
  }
}

void engine_add_background_map(map_t *map) {
  // error_if(engine.background_maps_len >= ENGINE_MAX_BACKGROUND_MAPS,
  // "BACKGROUND_MAPS_MAX reached");
  engine.background_maps[engine.background_maps_len++] = map;
}

void engine_set_collision_map(map_t *map) { engine.collision_map = map; }

void engine_set_scene(scene_t *scene) { scene_next = scene; }

void engine_update(void) {
  double time_frame_start = platform_now();

  // // Do we want to switch scenes?
  if (scene_next) {
    is_running = false;
    if (scene && scene->cleanup) {
      scene->cleanup();
    }

    // 	textures_reset(init_textures_mark);
    // 	images_reset(init_images_mark);
    // 	sound_reset(init_sounds_mark);
    // 	bump_reset(init_bump_mark);
    entities_reset();
    engine_maps_reset();

    engine.background_maps_len = 0;
    engine.collision_map = NULL;
    engine.time = 0;
    engine.frame = 0;
    engine.viewport = vec2(0, 0);

    scene = scene_next;
    if (scene->init) {
      scene->init();
    }
    scene_next = NULL;
  }
  is_running = true;

  error_if(scene == NULL, "No scene set");

  double time_real_now = platform_now();
  double real_delta = time_real_now - engine.time_real;
  engine.time_real = time_real_now;
  engine.tick = min(real_delta * engine.time_scale, ENGINE_MAX_TICK);
  engine.time += engine.tick;
  engine.frame++;

  // alloc_pool()
  {
    if (scene->update) {
      scene->update();
    } else {
      scene_base_update();
    }

    engine.perf.update = platform_now() - time_real_now;

    render_frame_prepare();

    if (scene->draw) {
      scene->draw();
    } else {
      scene_base_draw();
    }

    render_frame_end();
    engine.perf.draw = (platform_now() - time_real_now) - engine.perf.update;
  }

  input_clear();
  // temp_alloc_check();

  engine.perf.draw_calls = render_draw_calls();
  engine.perf.total = platform_now() - time_frame_start;
}

bool engine_is_running(void) { return is_running; }

void engine_resize(vec2i_t size) { render_resize(size); }

void scene_base_update(void) { entities_update(); }

void scene_base_draw(void) {
  vec2_t px_viewport = render_snap_px(engine.viewport);

  // Background maps
  for (int i = 0; i < engine.background_maps_len; i++) {
    if (!engine.background_maps[i]->foreground) {
      map_draw(engine.background_maps[i], px_viewport);
    }
  }

  entities_draw(px_viewport);

  // Foreground maps
  for (int i = 0; i < engine.background_maps_len; i++) {
    if (engine.background_maps[i]->foreground) {
      map_draw(engine.background_maps[i], px_viewport);
    }
  }
}