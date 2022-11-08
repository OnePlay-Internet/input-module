#include "config.h"
#include "platform/common.h"

using namespace std::chrono_literals;

namespace config {
input_t input {
  {
    { 0x10, 0xA0 },
    { 0x11, 0xA2 },
    { 0x12, 0xA4 },
  },
  2s,                                         // back_button_timeout
  500ms,                                      // key_repeat_delay
  std::chrono::duration<double> { 1 / 24.9 }, // key_repeat_period

  {
    platf::supported_gamepads().front().data(),
    platf::supported_gamepads().front().size(),
  }, // Default gamepad
};
} // namespace config
