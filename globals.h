// Author: Daniel Abreo

#ifndef GLOBALS_H
#define GLOBALS_H

const int BOARD_SIZE = 8;

enum Color {
    NONE = 0,
    WHITE = 1,
    RED = 2
};

enum JUMP_TYPE {
    JUMP_NONE,
    JUMP_NORMAL,
    JUMP_CAPTURE
};

std::string PATH_COLORS[] = {"\x1b[32m", "\x1b[33m", "\x1b[34m", "\x1b[35m","\x1b[30m"};
std::string PATH_PIECES[] = {"🟢","🟡","🔵","🟣","⚫️"};

#endif // GLOBALS_H
