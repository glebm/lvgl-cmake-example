#include <lvgl.h>

extern "C" int AppMain(int /*argc*/, char ** /*argv*/) {
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57),
	                          LV_PART_MAIN);
	lv_obj_t *label = lv_label_create(lv_screen_active());
	lv_label_set_text(label, "Hello world");
	lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff),
	                            LV_PART_MAIN);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	return 0;
}
