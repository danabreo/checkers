// Author: Daniel Abreo

#include "globals.h"

class Piece {
public:
    Piece() : color(NONE), king(false) {}
    Piece(Color color) : color(color), king(false) {}
    void setKing() {
        king = true;
    }
    Color getColor() const {
        return color;
    }
    bool isKing() const {
        return king;
    }
    Color getOppositeColor() {
        return (color==RED) ? WHITE : RED;
    }
    void remove() {
        color = NONE;
    }

private:
    Color color;
    bool king;
};
