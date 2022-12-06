#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <utility>
#include <iostream>
#include <functional>

#include "input_module.h"
#include "input_packet.h"
#include "utility.h"

/**
 * Test cases:
 * - Relative mouse position input
 * - Absolute mouse position input
 * - Mouse button input
 * - Mouse scroll input
 * - Keyboard input
 * - Game controller input
*/

class test_runner {
public:
    void set_test_result(const std::string& name, bool success, const std::string& reason = "") {
        results[name] = std::make_pair(success, reason);
    }

    void print_test_result() const {
        for(const auto &[test_name, test_result] : results) {
            const auto &[success, reason] = test_result;
            std::cout << test_name << ": ";
            std::cout << (success ? "success" : "failed");
            if(!success) {
                std::cout << ": " << reason; 
            }
            std::cout << std::endl;
        }
    }
private:
    std::unordered_map<std::string, std::pair<bool, std::string>> results;
};

using test_case_f = std::function<void(void*)>;

void test_rel_mouse(void*);
void test_abs_mouse(void*);
void test_mouse_button(void*);
void test_mouse_scroll(void*);
void test_keyboard(void*);
void test_game_controller(void*);

int main(int argc, char *argv[]) {
    std::vector<test_case_f> test_cases = {
        test_rel_mouse,
        // test_abs_mouse,
        // test_mouse_button,
        // test_mouse_scroll,
        // test_keyboard,
        // test_game_controller
    };

    test_runner t;
    for(const auto& f : test_cases) {
        f(&t);
    }
    t.print_test_result();
    return 0;
}

void test_rel_mouse(void* t) {
    std::cout << "Running test_rel_mouse" << std::endl;
    test_runner *runner = (test_runner *)t;

    inpmod_screen_cfg_t screen_cfg;
    screen_cfg.height = 1080;
    screen_cfg.width = 1920;

    std::cout << "Init input module" << std::endl;
    inpmod_t* in = inpmod_init(&screen_cfg);
    inpmod_log_level(in, 0);

    std::cout << "Mouse move packet" << std::endl;
    NV_REL_MOUSE_MOVE_PACKET packet;
    uint32_t packetType = PACKET_TYPE_REL_MOUSE_MOVE;
    int magic = MOUSE_MOVE_REL_MAGIC;
    packet.header.packetType = util::endian::big(packetType);
    packet.magic = util::endian::little(magic);

    // move right
    packet.deltaX = util::endian::big((short)5); 
    packet.deltaY = util::endian::big((short)0);
    std::cout << "Send packet 1" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // move down
    packet.deltaX = util::endian::big((short)0); 
    packet.deltaY = util::endian::big((short)5);
    std::cout << "Send packet 2" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // move left
    packet.deltaX = util::endian::big((short)-5); 
    packet.deltaY = util::endian::big((short)0);
    std::cout << "Send packet 3" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // move up
    packet.deltaX = util::endian::big((short)0); 
    packet.deltaY = util::endian::big((short)-5);
    std::cout << "Send packet 4" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    inpmod_wait(in, 30);

    std::cout << "Deinit input module" << std::endl;
    inpmod_deinit(in);

    std::cout << "Set test result" << std::endl;
    runner->set_test_result("test_rel_mouse", true);
}

