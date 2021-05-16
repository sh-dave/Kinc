#include <kinc/graphics4/graphics.h>
#include <kinc/input/gamepad.h>
#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/system.h>
#include <kinc/video.h>

namespace {
	int mouseX, mouseY;
	bool keyboardShown = false;
}

void Kinc_Mouse_GetPosition(int window, int *x, int *y) {
	*x = mouseX;
	*y = mouseY;
}

void kinc_keyboard_show() {
	keyboardShown = true;
}

void kinc_keyboard_hide() {
	keyboardShown = false;
}

bool kinc_keyboard_active() {
	return keyboardShown;
}

void kinc_vibrate(int ms) {}

const char* kinc_system_id() {
	return "macOS";
}

namespace {
	const char* videoFormats[] = {"ogv", nullptr};
}

const char** kinc_video_formats() {
	return ::videoFormats;
}

void kinc_set_keep_screen_on(bool on) {}

#include <mach/mach_time.h>

double kinc_frequency() {
	mach_timebase_info_data_t info;
	mach_timebase_info(&info);
	return (double)info.denom / (double)info.numer / 1e-9;
}

kinc_ticks_t kinc_timestamp() {
	return mach_absolute_time();
}

void kinc_login() {

}

void kinc_unlock_achievement(int id) {
	
}

bool kinc_gamepad_connected(int num) {
	return true;
}

void kinc_gamepad_rumble(int gamepad, float left, float right) {
	
}