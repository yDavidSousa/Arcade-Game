
typedef struct vector2 {
    float x, y;
} vector2_t;

static const vector2_t VECTOR_ONE = {1,1};
static const vector2_t VECTOR_RIGHT = {1,0};
static const vector2_t VECTOR_UP = {0,1};
static const vector2_t VECTOR_ZERO = {0,0};

vector2_t vector2_sum(vector2_t a, vector2_t b);

vector2_t vector2_subtract(vector2_t a, vector2_t b);

vector2_t vector2_multiply(vector2_t a, vector2_t b);

vector2_t vector2_multiply_scale(vector2_t a, float b);

vector2_t divideByScalar(vector2_t a, float b);

float vector2_magnitude(vector2_t a);

vector2_t vector2_normalize(vector2_t a);

float vector2_distance(vector2_t a, vector2_t b);

//TODO: finish this function
int vector2_equals(vector2_t a, vector2_t b);