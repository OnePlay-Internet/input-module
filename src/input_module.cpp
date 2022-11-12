#include <chrono>
#include <thread>
#include <string_view>

#include "main.h"
#include "input_module.h"
#include "input_impl.h"

safe::mail_t mail::man;
bool display_cursor = true;
util::ThreadPool task_pool;

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
    inpmod_screen_cfg_t* screen_cfg;
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
    inpmod_inst->screen_cfg = new inpmod_screen_cfg_t(*screen_cfg);
    input::touch_port_event->raise(input::make_port(inpmod_inst->screen_cfg));

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
    if(screen_cfg) {
        delete in->screen_cfg;
        in->screen_cfg = new inpmod_screen_cfg_t(*screen_cfg);
        input::touch_port_event->raise(input::make_port(in->screen_cfg));
    }
    std::vector<uint8_t> input_data;
    input_data.reserve(len);
    input_data.assign(data, data + len);
    input::print(input_data.data());
    input::passthrough(input::input_inst, std::move(input_data));
    return 0;
}

int inpmod_log_level(inpmod_t *in, int level) {
    if(in == nullptr || !in->init)
        return -1;
    set_log_level(level);
    return 0;
}

int inpmod_wait(inpmod_t *in, int sec) {
    if(in == nullptr || !in->init)
        return -1;
    auto start = std::chrono::steady_clock::now();
    while(task_pool.next() != std::nullopt) {
        auto now = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::seconds>(now - start).count() > sec) {
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}