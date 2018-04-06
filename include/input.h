
typedef enum KEY_STATE {
    NORMAL,
    PRESSED,
    RELEASED
} KEY_STATE_T;

typedef struct input_state {
    KEY_STATE_T up;
    KEY_STATE_T down;
    KEY_STATE_T left;
    KEY_STATE_T right;
    KEY_STATE_T esc;
} input_state_t;

void updateInput(input_state_t* inputState);