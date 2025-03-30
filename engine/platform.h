#ifndef PLATFORM_H
#define PLATFORM_H

#include "../libs/pl_json.h"
#include "input.h"
#include "types.h"
#include "utils.h"

// The window title, if applicable
#if !defined(WINDOW_TITLE)
#define WINDOW_TITLE "High Impact Game"
#endif

// The default window size, if applicable
#if !defined(WINDOW_WIDTH) || !defined(WINDOW_HEIGHT)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#endif

// The name of your company or handle. This may be used for the userdata
// directory, so it should not contain any special characters
#if !defined(GAME_VENDOR)
#define GAME_VENDOR "phoboslab"
#endif

// The name of your game. This may be used for the userdata directory, so it
// should not contain any special characters
#if !defined(GAME_NAME)
#define GAME_NAME "high_impact_game"
#endif

#if !defined(PLATFORM_VSYNC)
#define PLATFORM_VSYNC 1
#endif

// The max path length when loading/storing files
#if !defined(PLATFORM_MAX_PATH)
#define PLATFORM_MAX_PATH 512
#endif

// Return the current size of the window or render area in real pixels
vec2i_t platform_screen_size(void);

// Return the current time in seconds since program start
double platform_now(void);

// Whether the program is in fullscreen mode
bool platform_get_fullscreen(void);

// Set the fullscreen mode
void platform_set_fullscreen(bool fullscreen);

// Returns the samplerate of the audio output
uint32_t platform_samplerate(void);

// Load a file into temp memory. Must be freed via temp_free()
uint8_t *platform_load_asset(const char *name, uint32_t *bytes_read);

// Load a json file into temp memory. Must be freed via temp_free()
json_t *platform_load_asset_json(const char *name);

// Return the path to the current executable, bump allocated. Returns NULL
// on failure.
char *platform_executable_path(void);

// Return the directory part of path. A directory separator (/) is appended at
// the end.
char *platform_dirname(char *path);

// Load a file from the userdata directory into temp memory. Must be freed via
// temp_free(). This can be used for save games or configuration.
uint8_t *platform_load_userdata(const char *name, uint32_t *bytes_read);

// Store a file into the userdata directory.
uint32_t platform_store_userdata(const char *name, void *bytes, int32_t len);

// Exit the program
void platform_exit(void);

// Sets the audio mix callback; done by the engine
void platform_set_audio_mix_cb(void (*cb)(float *buffer, uint32_t len));

#if defined(RENDER_SOFTWARE)
rgba_t *platform_get_screenbuffer(int32_t *pitch);
#endif

#if defined(RENDER_METAL)
void *platform_get_metal_layer(void);
#endif

void platform_prepare_frame(void);
void platform_end_frame(void);

#endif // PLATFORM_H