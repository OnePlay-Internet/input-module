#ifndef SUNSHINE_CONFIG_H
#define SUNSHINE_CONFIG_H

#include <bitset>
#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace config {

struct input_t {
  std::unordered_map<int, int> keybindings;

  std::chrono::milliseconds back_button_timeout;
  std::chrono::milliseconds key_repeat_delay;
  std::chrono::duration<double> key_repeat_period;

  std::string gamepad;
};

extern input_t input;
} // namespace config
#endif
