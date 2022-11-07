// Created by loki on 6/20/19.

#ifndef SUNSHINE_INPUT_H
#define SUNSHINE_INPUT_H

#if 0
#include <functional>

#include "platform/common.h"
#include "thread_safe.h"

namespace input {
struct input_t;

void print(void *input);
void reset(std::shared_ptr<input_t> &input);
void passthrough(std::shared_ptr<input_t> &input, std::vector<std::uint8_t> &&input_data);


[[nodiscard]] std::unique_ptr<platf::deinit_t> init();

std::shared_ptr<input_t> alloc(safe::mail_t mail);

struct touch_port_t : public platf::touch_port_t {
  int env_width, env_height;

  // Offset x and y coordinates of the client
  float client_offsetX, client_offsetY;

  float scalar_inv;
};
} // namespace input
#endif

int input_init();

#endif // SUNSHINE_INPUT_H
