#include <SDL.h>
#include <SDL_main.h>
#include <lvgl.h>

#include <cstdint>

namespace {

SDL_LogPriority LvLogLevelToSdlLogPriority(lv_log_level_t log_level) {
	switch (log_level) {
		case LV_LOG_LEVEL_TRACE: return SDL_LOG_PRIORITY_VERBOSE;
		case LV_LOG_LEVEL_INFO: return SDL_LOG_PRIORITY_INFO;
		case LV_LOG_LEVEL_WARN: return SDL_LOG_PRIORITY_WARN;
		case LV_LOG_LEVEL_ERROR: return SDL_LOG_PRIORITY_ERROR;
		case LV_LOG_LEVEL_USER: return SDL_LOG_PRIORITY_INFO;
		default: return SDL_LOG_PRIORITY_CRITICAL;
	}
}

void SdlLogImpl(lv_log_level_t log_level, const char *buf) {
	if (log_level == LV_LOG_LEVEL_NONE) return;
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
	               LvLogLevelToSdlLogPriority(log_level), "%s", buf);
}
}  // namespace

extern "C" int AppMain(int argc, char **argv);

int main(int argc, char **argv) {
	lv_init();
	lv_log_register_print_cb(SdlLogImpl);
	lv_fs_stdio_init();

	lv_display_t *disp = lv_sdl_window_create(640, 480);
	if (disp == nullptr) {
		LV_LOG_ERROR("lv_sdl_window_create failed");
		lv_deinit();
		return 1;
	}

	lv_group_t *g = lv_group_create();
	lv_group_set_default(g);

	lv_sdl_mouse_create();
	lv_indev_t *mousewheel = lv_sdl_mousewheel_create();
	lv_indev_set_group(mousewheel, lv_group_get_default());

	lv_indev_t *keyboard = lv_sdl_keyboard_create();
	lv_indev_set_group(keyboard, lv_group_get_default());

	lv_sdl_window_set_zoom(disp, 2);

	if (const int result = AppMain(argc, argv); result != 0) {
		lv_deinit();
		return result;
	}

	while (true) {
		uint32_t delay = lv_timer_handler();
		if (delay < 1) delay = 1;
		SDL_Delay(delay);
	}
	return 0;
}
