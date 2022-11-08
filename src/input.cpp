#include "input.h"
#include "input_impl.h"

typedef struct input_t_ {
    int i;
    void *input_impl;
} input_t;

input_t* input_init(input_config_t* iconfig) {
    return nullptr;
}

int input_deinit(input_t *in) {
    return 0;
}

int input_reset(input_t *in) {
    return 0;
}

int input_print(input_t *in, void *data) {
    return 0;
}

int input_passthrough(input_t *in, uint8_t *data) {
    return 0;
}