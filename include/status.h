
/* Components represents the data a game object can have
 * Entities represent a game object as an aggregation of components
 * */

#define ENTITY_COUNT 20
#define MOVEMENT_MASK (COMPONENT_DISPLACEMENT | COMPONENT_APPEARANCE)

typedef enum {
// Component mask (Bit field)
            COMPONENT_NONE = 0,
    COMPONENT_DISPLACEMENT = 1 << 0,
    COMPONENT_VELOCITY = 1 << 1,
    COMPONENT_APPEARANCE = 1 << 2
} Component;

typedef struct displacement_component {
    float x;
    float y;
} displacement_t;

typedef struct velocity_component {
    float x;
    float y;
} velocity_t;

typedef struct appearance_component {
    char name;
} appearance_t;

typedef struct world {
    int mask[ENTITY_COUNT];
    displacement_t displacement[ENTITY_COUNT];
    velocity_t velocity[ENTITY_COUNT];
    appearance_t appearance[ENTITY_COUNT];
} world_t;

unsigned int createEntity(world_t *world);

void destroyEntity(world_t *world, unsigned int entity);

unsigned int createTree(world_t *world, float x, float y);

void movementFunction(world_t *world);