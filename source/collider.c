
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

float clamp(float value, float min, float max){
    return mathMin(mathMin(value, mathMax(min, max)),mathMax(min, max));
}

bool_t inRange(float value, float min, float max){
    if (value >= mathMin(min, max) && value <= mathMax(min, max))
        return TRUE;
    else
        return FALSE;
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

bool_t boxCollision(SDL_Rect box1, SDL_Rect box2){
    return rangeIntersect(box1.x, box1.x + box1.w, box2.x, box2.x + box2.w)
           && rangeIntersect(box1.y, box1.y + box1.h, box2.y, box2.y + box2.h)? TRUE : FALSE;
}

bool_t boxPointCollision(SDL_Rect box, point_t point){
    return inRange(point.x, box.x, box.x + box.w) && inRange(point.y, box.y, box.y + box.h) ? TRUE : FALSE;
}