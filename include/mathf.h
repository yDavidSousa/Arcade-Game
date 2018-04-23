#include <vector2.h>

float mathf_min(float a, float b);

float mathf_max(float a, float b);

float mathf_clamp(float value, float min, float max);

int mathf_range(float value, float min, float max);

float mathf_abs(float value);

double mathf_distance(vector2_t a, vector2_t b);

int mathf_range_intersect(float minA, float maxA, float minB, float maxB);