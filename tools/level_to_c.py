#!python

import json

with open('./assets/levels/biolab-1.json') as f:
	json_string = f.read()

# Parse JSON string into a Python dictionary
data = json.loads(json_string)

# print(data['entities'])

for e in data['entities']:
	print("{")
	print(f'  char *type_name = "{e["type"]}";')
	print(f'  entity_type_t type = entity_type_by_name(type_name);');
	print(f'  vec2_t pos = vec2({e["x"]}, {e["y"]});');
	print(f'  entity_t *ent = entity_spawn(type, pos);');
	# if 'settings' in e:
	# 	js = json.dumps(e['settings']).replace('"', '\\"')
	# 	print("  {")
	# 	print(f'    char* settings = "{js}";')
	# 	print(f'    json_t *json = json_parse(settings, strlen(settings));');
	# 	print(f'    free(json);');
	# 	print("  }")
	print("}\n")

# print(data['maps'])
for m in data['maps']:
	# print("{")
	# js = json.dumps(m).replace('"', '\\"')
	# print(f'    char* maps = "{js}";')
	# print(f'    json_t *json = json_parse(maps, strlen(maps));');
	# print(f'    map_t *map = map_from_json(json);');
	# print(f'    free(json);');
	# print(f'    char *name = "{m["name"]}";');
	# print(f'    if (str_equals(name, "collision")) ');
	# print(f'      engine_set_collision_map(map);');
	# print(f'    else');
	# print(f'      engine_add_background_map(map);');
	# print("}\n")
	print("{")
	print(f'    map_t *map = malloc(sizeof(map_t));')
	print(f'    memset(map, 0, sizeof(map_t));')
	print(f'    map->size.x = {m["width"]};');
	print(f'    map->size.y = {m["height"]};');
	print(f'    map->tile_size = {m["tilesize"]};');
	print(f'    map->distance = {m["distance"]};');
	# if "foreground" in m:
	# 	print(f'    map->foreground = {m["foreground"]};');
	# if "repeat" in m:
	# 	print(f'    map->repeat = {m["repeat"]};');
	print(f'    char *name = "{m["name"]}";');
	print(f'    strcpy(map->name, name);');
	print(f'    if (str_equals(name, "collision")) ');
	print(f'      engine_set_collision_map(map);');
	print(f'    else');
	print(f'      engine_add_background_map(map);');
	if 'tilesetName' in m and not m['tilesetName'] is None:
		print(f'    map->tileset = image("{m["tilesetName"]}");');
	data = json.dumps(m["data"]).replace('[', '{').replace(']','}')
	data = data.replace('{{', '{')
	data = data.replace('}}', '}')
	data = data.replace('}, {', ',')
	print(f'    const uint16_t data[] = {data};');
	print(f'    const int dataSize = sizeof(uint16_t) * map->size.x * map->size.y;');
	print(f'    map->data = malloc(dataSize);');
	print(f'    int index = 0;');
	print( "    for (int y = 0; y < map->size.y; y++) {");
	print( "    for (int x = 0; x < map->size.x; x++, index++) {");
	print( "    map->max_tile = max(map->max_tile, map->data[index]);");
	print( "    map->data[index] = data[index];");
	print( "    } }");
	# print(f'    memcpy(map->data, &data, dataSize);');
	print("}\n")