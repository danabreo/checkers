// Author: Daniel Abreo
// Run: g++ -std=c++14 main.cpp && ./a.out

#include "game.h"

int main() {
    cout << "Welcome to GUI checkers!" << endl; 
    Game game;
    game.run();
    cout << "Game Over!" << endl;
}