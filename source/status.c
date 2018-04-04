#include <stdio.h>
#include <status.h>

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