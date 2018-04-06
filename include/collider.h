typedef enum bool {
    FALSE = 0,
    TRUE = 1
} bool_t;

typedef struct point{
    float x, y;
} point_t;

typedef struct circle_collider{
    point_t center;
    float radius;
} circle_collider_t;

//Check collision between circles
bool_t circleCollision(circle_collider_t circle1, circle_collider_t circle2);

//Check collision between circle and point
bool_t circlePointCollision(circle_collider_t circle, point_t point);

//Check collision between boxes
bool_t boxCollision(SDL_Rect box1, SDL_Rect box2);

//Check collision between box and point
bool_t boxPointCollision(SDL_Rect box, point_t point);