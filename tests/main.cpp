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
#include "matoya.h"


// Your top level application context
struct context {
	MTY_App *app;
	bool quit;
};

// This function will fire for each event
static void event_func(const MTY_Event *evt, void *opaque)
{
	struct context *ctx = (struct context *)opaque;

	MTY_PrintEvent(evt);

	if (evt->type == MTY_EVENT_CLOSE)
		ctx->quit = true;
}

// This function fires once per "cycle", either blocked by a
// call to MTY_WindowPresent or limited by MTY_AppSetTimeout
static bool app_func(void *opaque)
{
	struct context *ctx = (struct context *)opaque;

	MTY_WindowPresent(ctx->app, 0);

	return !ctx->quit;
}

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

    // Set up the application object and attach it to your context
	struct context ctx = {0};
	ctx.app = MTY_AppCreate(app_func, event_func, &ctx);
	if (!ctx.app)
		return 1;

	// Create a window
	MTY_WindowCreate(ctx.app, "My Window", NULL, 0);

	// Set the graphics API to OpenGL
	MTY_WindowSetGFX(ctx.app, 0, MTY_GFX_GL, true);

	// Run the app -- blocks until your app_func returns false
	MTY_AppRun(ctx.app);
	MTY_AppDestroy(&ctx.app);

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
    packet.deltaX = util::endian::big((short)500); 
    packet.deltaY = util::endian::big((short)0);
    std::cout << "Send packet 1" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // move down
    packet.deltaX = util::endian::big((short)0); 
    packet.deltaY = util::endian::big((short)500);
    std::cout << "Send packet 2" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // move left
    packet.deltaX = util::endian::big((short)-500); 
    packet.deltaY = util::endian::big((short)0);
    std::cout << "Send packet 3" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    // move up
    packet.deltaX = util::endian::big((short)0); 
    packet.deltaY = util::endian::big((short)-500);
    std::cout << "Send packet 4" << std::endl;
    inpmod_print(in, (uint8_t *)&packet);
    inpmod_passthrough(in, (uint8_t *)&packet, sizeof(packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    inpmod_wait(in, 30);

    std::cout << "Deinit input module" << std::endl;
    inpmod_deinit(in);

    std::cout << "Set test result" << std::endl;
    runner->set_test_result("test_rel_mouse", true);
}

