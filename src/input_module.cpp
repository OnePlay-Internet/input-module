#include <string_view>

#include "main.h"
#include "input_module.h"
#include "input_impl.h"

safe::mail_t mail::man;
bool display_cursor;

int _ = log_init();

namespace input {
std::unique_ptr<platf::deinit_t> input_deinit_guard = nullptr;
std::shared_ptr<input_t> input_inst = nullptr;

// absolute mouse coordinates require that the dimensions of the screen are known
safe::mail_raw_t::event_t<input::touch_port_t> touch_port_event;

input::touch_port_t make_port(inpmod_screen_cfg_t* screen_cfg) {
    auto port = input::touch_port_t{};
    port.offset_x = screen_cfg->offset_x;
    port.offset_y = screen_cfg->offset_y;
    port.width = screen_cfg->width;
    port.height = screen_cfg->height;
    port.env_width = screen_cfg->env_width;
    port.env_height = screen_cfg->env_height;
    port.client_offsetX = screen_cfg->client_offsetX;
    port.client_offsetY = screen_cfg->client_offsetY;
    return port;
}
} // namespace input

typedef struct inpmod_t_ {
    bool init = false;
} inpmod_t;

inpmod_t* inpmod_inst = nullptr;

inpmod_t* inpmod_init(inpmod_screen_cfg_t* screen_cfg, inpmod_key_cfg_t* key_cfg) {
    if(inpmod_inst != nullptr && inpmod_inst->init)
        return inpmod_inst;

    input::input_deinit_guard = input::init();
    mail::man = std::make_shared<safe::mail_raw_t>();
    input::touch_port_event = mail::man->event<input::touch_port_t>(mail::touch_port);
    input::input_inst = input::alloc(mail::man);

    display_cursor = true;
    task_pool.start(1);

    inpmod_inst = new inpmod_t{};
    inpmod_inst->init = true;

    return inpmod_inst;
}

int inpmod_deinit(inpmod_t *in) {
    if(in == nullptr || !in->init)
        return -1;

    input::input_deinit_guard.reset();
    input::input_deinit_guard = nullptr;

    input::input_inst.reset();
    input::input_inst = nullptr;

    input::touch_port_event->reset();
    mail::man->cleanup();

    task_pool.stop();
    task_pool.join();

    return 0;
}

int inpmod_reset(inpmod_t *in) {
    if(in == nullptr || !in->init)
        return -1;
    input::reset(input::input_inst);
    return 0;
}

int inpmod_print(inpmod_t *in, void *data) {
    if(in == nullptr || !in->init)
        return -1;
    input::print(data);
    return 0;
}

int inpmod_passthrough(inpmod_t *in, uint8_t *data, int len, inpmod_screen_cfg_t* screen_cfg) {
    if(in == nullptr || !in->init)
        return -1;
    if(screen_cfg)
        input::touch_port_event->raise(input::make_port(screen_cfg));
    std::vector<uint8_t> input_data;
    input_data.reserve(len);
    input_data.assign(data, data + len);
    input::passthrough(input::input_inst, std::move(input_data));
    return 0;
}