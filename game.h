// Author: Daniel Abreo

#include <algorithm>
#include <vector>

#include "board.h"

using namespace std;

class Game {
public:
    Game() : curColor(RED) {}
    void run() {
        while (inProgress())
            processTurn();
    }

private:
    bool inProgress() {
        return (board.numPieces(RED) != 0 && board.numPieces(WHITE) != 0);
    }
    void processTurn() {
        // Display current board state
        cout << board << endl;

        // Prompt user for the piece they would like to move, until a valid piece is selected
        string selectedPieceLocation;
        vector<vector<pair<int,int>>> paths;
        do {
            cout << "Player " << curColor << ", what piece do you want to move (example: F3): ";
            cin >> selectedPieceLocation;
            transform(selectedPieceLocation.begin(), selectedPieceLocation.end(), selectedPieceLocation.begin(), ::toupper);
            if (board.validPiece(curColor, selectedPieceLocation))
                paths = board.generatePaths(selectedPieceLocation);
        } while (paths.size() == 0);

        // Prompt user for the path they would like to move the piece thorugh
        cout << "Here are the paths you can move through:" << endl;
        for (int i = 0; i < paths.size(); ++i) {
            cout << PATH_COLORS[i%(sizeof(PATH_COLORS)/sizeof(PATH_COLORS[0]))] << "  " << i << ": ";
            for (int j = 0; j < paths[i].size(); ++j) {
                cout << board.getLocationFromIndex(paths[i][j]) << " ";
            } 
            cout << "\x1b[0m " << endl;
        }
        cout << "===========================" << endl;
        board.printBoardPaths(paths);
        int selectedPath;
        while (true) {
            cout << "What path do you want to move through (example: 0): ";
            cin >> selectedPath;
            if (!cin.fail() && selectedPath >= 0 && selectedPath < paths.size())
                break;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Execute the selected move
        board.executePath(paths[selectedPath]);

        // Switch players
        pair<int,int> curPieceIndex = paths[selectedPath][paths[selectedPath].size()-1];
        curColor = board.getPiece(curPieceIndex.first, curPieceIndex.second).getOppositeColor();
    }

    Color curColor;
    Board board;
};
