#include <math.h>
#include <SDL.h>
#include <collider.h>

//TODO: Put this functions in other file(math)
float mathMin(float a, float b){
    return (a <= b) ? a : b;
}

float mathMax(float a, float b){
    return (a >= b) ? a : b;
}

float mathClamp(float value, float min, float max){
    return mathMin(mathMin(value, mathMax(min, max)),mathMax(min, max));
}

int mathRange(float value, float min, float max){
    return (value >= mathMin(min, max) && value <= mathMax(min, max)) ? 1 : 0;
}

float mathAbs(float f){
    return (f < 0) ? -f : f ;
}

double distanceBetweenPoints(point_t a, point_t b){
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

bool_t rangeIntersect(float minA, float maxA, float minB, float maxB){
    return (mathMax(minA, maxA) >= mathMin(minB, maxB) && mathMin(minA, maxA) <= mathMax(minB, maxB)) ? TRUE : FALSE;
}

bool_t circleCollision(circle_collider_t circle1, circle_collider_t circle2){
    return circle1.radius + circle2.radius >= distanceBetweenPoints(circle1.center, circle2.center) ? TRUE : FALSE;
}

bool_t circlePointCollision(circle_collider_t circle, point_t point){
    return distanceBetweenPoints(circle.center, point) < circle.radius ? TRUE : FALSE;
}

bool_t boxCollision(box_collider_t box1, box_collider_t box2){
    return rangeIntersect(box1.min.x, max_boxcollider(box1).x, box2.min.x, max_boxcollider(box2).x)
           && rangeIntersect(box1.min.y, max_boxcollider(box1).y, box2.min.y, max_boxcollider(box2).y)? TRUE : FALSE;
}

bool_t boxPointCollision(SDL_Rect box, point_t point){
    return mathRange(point.x, box.x, box.x + box.w) && mathRange(point.y, box.y, box.y + box.h) ? TRUE : FALSE;
}


box_collider_t create_boxcollider(vector2_t center, vector2_t extents){
    box_collider_t boxcollider = {};
    boxcollider.center = center;
    boxcollider.extents = extents;
    update_boxcollider(&boxcollider);
    return boxcollider;
}

void update_boxcollider(box_collider_t *boxcollider){
    boxcollider->min = min_boxcollider(*boxcollider);
    boxcollider->max = max_boxcollider(*boxcollider);
    boxcollider->size = size_boxcollider(*boxcollider);
}

box_collider_t minkowski_difference(box_collider_t boxA, box_collider_t boxB){
    vector2_t topLeft = vector2_subtract(boxA.min, boxB.max);
    vector2_t fullSize = vector2_sum(boxA.size, boxB.size);

    return create_boxcollider(vector2_sum(topLeft, divideByScalar(fullSize, 2)), divideByScalar(fullSize, 2));
}

vector2_t closest_point_bounds_to_point(box_collider_t boxcollider, vector2_t point){
    float minDist = mathAbs(point.x - boxcollider.min.x);
    vector2_t boundsPoint = {boxcollider.min.x, point.y};

    //Finish checking x axis
    if(mathAbs(boxcollider.max.x - point.x) < minDist){
        minDist = mathAbs(boxcollider.max.x - point.x);
        boundsPoint.x = boxcollider.max.x;
        boundsPoint.y = point.y;
    }

    //Move to y axis
    if(mathAbs(boxcollider.max.y - point.y) < minDist){
        minDist = mathAbs(boxcollider.max.y - point.y);
        boundsPoint.x = point.x;
        boundsPoint.y = boxcollider.max.y;
    }

    if(mathAbs(boxcollider.min.y - point.y) < minDist){
        minDist = mathAbs(boxcollider.min.y - point.y);
        boundsPoint.x = point.x;
        boundsPoint.y = boxcollider.min.y;
    }

    return boundsPoint;
}

vector2_t min_boxcollider(box_collider_t boxcollider){
    vector2_t result = {boxcollider.center.x - boxcollider.extents.x, boxcollider.center.y - boxcollider.extents.y};
    return result;
}

vector2_t max_boxcollider(box_collider_t boxcollider){
    vector2_t result = {boxcollider.center.x + boxcollider.extents.x, boxcollider.center.y + boxcollider.extents.y};
    return result;
}

vector2_t size_boxcollider(box_collider_t boxcollider){
    return vector2_multiply_scale(boxcollider.extents, 2);
}

//TODO: Set parameter color
void draw_box(box_collider_t boxcollider, SDL_Renderer *renderer){
    SDL_Rect boxToDraw = {};
    boxToDraw.x = (int)boxcollider.min.x;
    boxToDraw.y = (int)boxcollider.min.y;
    boxToDraw.w = (int)boxcollider.size.x;
    boxToDraw.h = (int)boxcollider.size.y;

    SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderDrawRect( renderer, &boxToDraw );
}

void draw_solid_box(box_collider_t boxcollider, SDL_Renderer *renderer){
    SDL_Rect boxToDraw = {};
    boxToDraw.x = (int)boxcollider.min.x;
    boxToDraw.y = (int)boxcollider.min.y;
    boxToDraw.w = (int)boxcollider.size.x;
    boxToDraw.h = (int)boxcollider.size.y;

    SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect(renderer, &boxToDraw);
}