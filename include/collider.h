#include <vector2.h>

//TODO: Remove this after
typedef enum bool {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef struct point{
    float x, y;
} point_t;

//TODO: Change all the structure for pivot
typedef struct boxcollider {
    vector2_t min, max;
    vector2_t center;
    vector2_t extents;
    vector2_t size;
} box_collider_t;

typedef struct circle_collider{
    point_t center;
    float radius;
} circle_collider_t;

/* Check collision between circles */
bool_t circleCollision(circle_collider_t circle1, circle_collider_t circle2);

/* Check collision between circle and point */
bool_t circlePointCollision(circle_collider_t circle, point_t point);

/* Check collision between boxes */
bool_t boxCollision(box_collider_t box1, box_collider_t box2);

/* Check collision between box and point */
bool_t boxPointCollision(SDL_Rect box, point_t point);

/* Test */
box_collider_t create_boxcollider(vector2_t center, vector2_t extents);

void update_boxcollider(box_collider_t *boxcollider);

vector2_t min_boxcollider(box_collider_t boxcollider);

vector2_t max_boxcollider(box_collider_t boxcollider);

vector2_t size_boxcollider(box_collider_t boxcollider);

/* Test */
box_collider_t minkowski_difference(box_collider_t boxA, box_collider_t boxB);

/* Return the penetration vector */
vector2_t closest_point_bounds_to_point(box_collider_t boxcollider, vector2_t point);

void draw_box(box_collider_t boxcollider, SDL_Renderer *renderer);

void draw_solid_box(box_collider_t boxcollider, SDL_Renderer *renderer);
