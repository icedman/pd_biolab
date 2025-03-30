
#define ENTITY_TYPES(TYPE)                                                     \
  TYPE(ENTITY_TYPE_BLOB, blob)                                                 \
  TYPE(ENTITY_TYPE_CRATE, crate)                                               \
  TYPE(ENTITY_TYPE_DEBRIS, debris)                                             \
  TYPE(ENTITY_TYPE_DELAY, delay)                                               \
  TYPE(ENTITY_TYPE_DROPPER, dropper)                                           \
  TYPE(ENTITY_TYPE_EARTHQUAKE, earthquake)                                     \
  TYPE(ENTITY_TYPE_END_HUB, end_hub)                                           \
  TYPE(ENTITY_TYPE_END_HUB_PLASMA, end_hub_plasma)                             \
  TYPE(ENTITY_TYPE_END_HUB_FADE, end_hub_fade)                                 \
  TYPE(ENTITY_TYPE_GLASS_DOME, glass_dome)                                     \
  TYPE(ENTITY_TYPE_GRUNT, grunt)                                               \
  TYPE(ENTITY_TYPE_HURT, hurt)                                                 \
  TYPE(ENTITY_TYPE_LEVEL_CHANGE, level_change)                                 \
  TYPE(ENTITY_TYPE_MINE, mine)                                                 \
  TYPE(ENTITY_TYPE_MOVER, mover)                                               \
  TYPE(ENTITY_TYPE_PARTICLE, particle)                                         \
  TYPE(ENTITY_TYPE_PLAYER, player)                                             \
  TYPE(ENTITY_TYPE_PROJECTILE, projectile)                                     \
  TYPE(ENTITY_TYPE_RESPAWN_POD, respawn_pod)                                   \
  TYPE(ENTITY_TYPE_SPIKE, spike)                                               \
  TYPE(ENTITY_TYPE_SPEWER, spewer)                                             \
  TYPE(ENTITY_TYPE_SPEWER_SHOT, spewer_shot)                                   \
  TYPE(ENTITY_TYPE_TEST_TUBE, test_tube)                                       \
  TYPE(ENTITY_TYPE_TRIGGER, trigger)                                           \
  TYPE(ENTITY_TYPE_VOID, void)

// All entity types share the same struct. Calling ENTITY_DEFINE() defines that
// struct with the fields required by high_impact and the additional fields
// specified here.

ENTITY_DEFINE(
    // Entity private data
    union {
      struct {
        bool in_jump;
        bool seen_player;
        float jump_timer;
      } blob;

      struct {
        int count;
        float duration;
        float duration_time;
        float emit_time;
      } debris;

      struct {
        entity_list_t targets;
        entity_ref_t triggered_by;
        bool fire;
        float delay;
        float delay_time;
      } delay;

      struct {
        float shoot_wait_time;
        float shoot_time;
        bool can_shoot;
      } dropper;

      struct {
        float duration;
        float strength;
        float time;
      } earthquake;

      struct {
        int stage;
      } end_hub;

      struct {
        float time;
        int index;
      } end_hub_plasma;

      struct {
        float time;
      } end_hub_fade;

      struct {
        float shoot_time;
        bool flip;
        bool seen_player;
      } grunt;

      struct {
        float damage;
      } hurt;

      struct {
        char *path;
      } level_change;

      struct {
        entity_list_t targets;
        int current_target;
        float speed;
      } mover;

      struct {
        float life_time;
        float fade_time;
      } particle;

      struct {
        float high_jump_time;
        float idle_time;
        bool flip;
        bool can_jump;
        bool is_idle;
      } player;

      struct {
        anim_def_t *anim_hit;
        bool has_hit;
        bool flip;
      } projectile;

      struct {
        bool activated;
      } respawn_pod;

      struct {
        float shoot_wait_time;
        float shoot_time;
        bool can_shoot;
        bool flip;
      } spike;

      struct {
        float shoot_wait_time;
        float shoot_time;
        bool can_shoot;
      } spewer;

      struct {
        int bounce_count;
      } spewer_shot;

      struct {
        entity_list_t targets;
        float delay;
        float delay_time;
        bool can_fire;
      } trigger;
    };);

// The entity_message_t is used with the entity_message() function. You can
// extend this as you wish.

typedef enum {
  EM_INVALID,
  EM_ACTIVATE,
} entity_message_t;