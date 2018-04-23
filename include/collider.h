#include <mathf.h>
#include <util.h>

//TODO: Change all the structure for pivot
typedef struct box_collider {
    vector2_t min, max;
    vector2_t extents;
    vector2_t size;
} box_collider_t;

typedef struct circle_collider{
    vector2_t center;
    float radius;
} circle_collider_t;

/* Check collision between circles */
int collider_circle_collision(circle_collider_t circleA, circle_collider_t circleB);

/* Check collision between boxes */
int collider_box_collision(box_collider_t boxA, box_collider_t boxB);

/* Check collision between circle and point */
int collider_circle_point_collision(circle_collider_t circle, vector2_t point);

/* Check collision between box and point */
int collider_box_point_collision(box_collider_t boxcollider, vector2_t point);

box_collider_t collider_new_box(float x, float y, float w, float h);

void collider_update_box(box_collider_t *boxcollider, float x, float y);

box_collider_t collider_minkowski_diff_box(box_collider_t boxA, box_collider_t boxB);

/* Return the penetration vector */
vector2_t collider_penetration_vector_box(box_collider_t boxcollider, vector2_t point);

void collider_render_box(box_collider_t boxcollider, SDL_Renderer *renderer, color_t color);

void collider_render_fill_box(box_collider_t boxcollider, SDL_Renderer *renderer, color_t color);
