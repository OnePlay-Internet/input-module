#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct input_t_ input_t;

typedef struct input_config_t_ {
    // Dimensions for touchscreen input
    int offset_x;
    int offset_y;
    int width;
    int height;
    int env_width;
    int env_height;
    // Offset x and y coordinates of the client
    float client_offsetX;
    float client_offsetY;
    float scalar_inv;

    // key bindings
    int* keybindings_key;
    int* keybindings_value;
    int keybindings_len;

    int64_t back_button_timeout; // in ms
    int64_t key_repeat_delay; // in ms
    double key_repeat_period;

    // supported gamepad: "x360", "ds4", "ps4" (ds4 == ps4)
    char gamepad[16];
} input_config_t;

input_t* input_init(input_config_t* iconfig);

int input_deinit(input_t *in);

int input_reset(input_t *in);

int input_print(input_t *in, void *data);

int input_passthrough(input_t *in, uint8_t *data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ONEPLAY_INPUT_MODULE_H