#include <math.h>
#include <vector2.h>

vector2_t vector2_sum(vector2_t a, vector2_t b){
    vector2_t result = {a.x + b.x, a.y + b.y};
    return result;
}

vector2_t vector2_subtract(vector2_t a, vector2_t b){
    vector2_t result = {a.x - b.x, a.y - b.y};
    return result;
}

vector2_t vector2_multiply(vector2_t a, vector2_t b){
    vector2_t result = {a.x * b.x, a.y * b.y};
    return result;
}

vector2_t vector2_multiply_scale(vector2_t a, float b){
    vector2_t result = {a.x * b, a.y * b};
    return result;
}

vector2_t divideByScalar(vector2_t a, float b){
    vector2_t result = {a.x / b, a.y / b};
    return result;
}

float vector2_magnitude(vector2_t a){
    return (float)sqrt(pow(a.x, 2) + pow(a.y, 2));
}

vector2_t vector2_normalize(vector2_t a){
    float magnitude = vector2_magnitude(a);
    vector2_t result = {a.x / magnitude, a.y / magnitude};
    return result;
}

float vector2_distance(vector2_t a, vector2_t b){
    vector2_t result = vector2_subtract(a, b);
    return (float)sqrt(pow(result.x, 2) + pow(result.y, 2));
}

//TODO: finish this function
int vector2_equals(vector2_t a, vector2_t b){
    return 0;
}