#include <stdio.h>
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

unsigned int createEntity(world_t *world) {
    unsigned int entity;
    for (entity = 0; entity < ENTITY_COUNT; entity++)
        if(world->mask[entity] == COMPONENT_NONE)
            return(entity);

    printf("Error! No more entities left!\n");
    return (ENTITY_COUNT);
}

void destroyEntity(world_t *world, unsigned int entity) {
    world->mask[entity] = COMPONENT_NONE;
}

unsigned int createTree(world_t *world, float x, float y) {
//In a real-world engine, your entities would likely be defined using external data files
    unsigned int entity = createEntity(world);

    world->mask[entity] = COMPONENT_DISPLACEMENT | COMPONENT_APPEARANCE;
    world->displacement[entity].x = x;
    world->displacement[entity].y = y;
    world->appearance[entity].name = 'T';

    return (entity);
}

void movementFunction(world_t *world) {
    unsigned int entity;
    displacement_t *d;
    velocity_t *v;

    for (entity = 0; entity < ENTITY_COUNT; ++entity) {
        //If entity has the same components that MOVEMENT_MASK, apply movement
        if((world->mask[entity] & MOVEMENT_MASK) == MOVEMENT_MASK)
        {
            d = &(world->displacement[entity]);
            v = &(world->velocity[entity]);

            v->y -= 0.98f;

            d->x += v->x;
            d->y += v->y;
        }
    }
}