#include <mathf.h>
#include <math.h>

float mathf_min(float a, float b){
    return (a <= b) ? a : b;
}

float mathf_max(float a, float b){
    return (a >= b) ? a : b;
}

float mathf_clamp(float value, float min, float max){
    return mathf_min(mathf_min(value, mathf_max(min, max)), mathf_max(min, max));
}

int mathf_range(float value, float min, float max){
    return (value >= mathf_min(min, max) && value <= mathf_max(min, max)) ? 1 : 0;
}

float mathf_abs(float value){
    return (value < 0) ? -value : value ;
}

double mathf_distance(vector2_t a, vector2_t b){
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

int mathf_range_intersect(float minA, float maxA, float minB, float maxB){
    return (mathf_max(minA, maxA) >= mathf_min(minB, maxB) && mathf_min(minA, maxA) <= mathf_max(minB, maxB)) ? 1 : 0;
}