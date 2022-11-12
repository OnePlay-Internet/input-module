#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct inpmod_t_ inpmod_t;

typedef struct inpmod_screen_cfg_t_ {
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
} inpmod_screen_cfg_t;

typedef struct inpmod_key_cfg_t_ {
    // key bindings
    int* keybindings_key;
    int* keybindings_value;
    int keybindings_len;

    int64_t back_button_timeout; // in ms
    int64_t key_repeat_delay; // in ms
    double key_repeat_period;

    // supported gamepad: "x360", "ds4", "ps4" (ds4 == ps4)
    char gamepad[16];
} inpmod_key_cfg_t;

inpmod_t* inpmod_init(inpmod_screen_cfg_t* screen_cfg = NULL, inpmod_key_cfg_t* key_cfg = NULL);

int inpmod_deinit(inpmod_t *in);

int inpmod_reset(inpmod_t *in);

int inpmod_print(inpmod_t *in, void *data);

int inpmod_passthrough(inpmod_t *in, uint8_t *data, int len, inpmod_screen_cfg_t* screen_cfg = NULL);

int inpmod_log_level(inpmod_t *in, int level);

int inpmod_wait(inpmod_t *in, int sec);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ONEPLAY_INPUT_MODULE_H