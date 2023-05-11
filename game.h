// Author: Daniel Abreo

#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

struct pair_hash {
    size_t operator () (const pair<int,int> &p) const {
        auto h1 = hash<int>{}(p.first);
        auto h2 = hash<int>{}(p.second);
        return h1 ^ h2;  
    }
};

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

string PATH_COLORS[] = {"\x1b[33m", "\x1b[34m", "\x1b[35m", "\x1b[36m"};

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

class Board {
public:
    Board() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (i<3 && (i+j)%2==1)
                    board[i][j] = Piece(WHITE);
                else if (i>4 && (i+j)%2==1)
                    board[i][j] = Piece(RED);
                else
                    board[i][j] = Piece(NONE);
            }
        }
    }
    vector<vector<pair<int,int>>> generatePaths(string location) {
        vector<vector<pair<int,int>>> paths;
        vector<pair<int,int>> curPath;
        pair<int,int> curNode = getIndexFromLocation(location);
        JUMP_TYPE jump_type = JUMP_NONE;
        Piece curPiece = getPiece(curNode.first,curNode.second);
        dfs(curNode, curPath, paths, jump_type, curPiece);
        return paths;
    }
    bool isValidMove(pair<int,int> start, int dest_row, int dest_col, Piece curPiece) {
        // Dest. location must be in bounds
        if (dest_row < 0 || dest_row >= BOARD_SIZE)
            return false;
        if (dest_col < 0 || dest_col >= BOARD_SIZE)
            return false;

        // Dest. location must be empty
        if (getPiece(dest_row, dest_col).getColor() != NONE)
            return false;

        // If start piece is white and not king, dest_row > start.first
        if (curPiece.getColor() == WHITE && !curPiece.isKing() && dest_row <= start.first)
            return false;
        // If start piece is red and not king, dest_row < start.first
        if (curPiece.getColor() == RED && !curPiece.isKing() && dest_row >= start.first)
            return false;

        // If move is a jump, jumped piece must be different color
        if (abs(start.first - dest_row) == 2) {
            Piece jumpedPiece = getPiece((start.first+dest_row)/2,(start.second+dest_col)/2);
            if (curPiece.getOppositeColor() != jumpedPiece.getColor())
                return false;
        }
        return true;
    }
    int numPieces(Color color) {
        int count = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j].getColor() == color)
                    ++count;
            }
        }
        return count;
    }
    bool validPiece(Color curColor, string location) {
        // Check length
        if (location.size() != 2)
            return false;

        // Check format, ex: A1, H8, C4
        if (!isalpha(location[0]) || location[0] < 'A' || location[0] > 'H')
            return false;
        if (!isnumber(location[1]) || location[1] < '1' || location[1] > '8')
            return false;

        // Check color
        pair<int,int> row_col = getIndexFromLocation(location);
        if (board[row_col.first][row_col.second].getColor() != curColor)
            return false;

        return true;
    }
    Piece getPiece(int row, int col) const {
        return board[row][col];
    }
    friend ostream &operator<<(ostream &output, const Board &B) {
        output << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i)
            output << " " << char('0'+i+1) << " ";
        output << endl << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i)
            output << "---";
        output << endl;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            output << char('A'+i) << " |";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                Color curColor = B.getPiece(i,j).getColor();
                output << " ";
                if (curColor==RED)
                    output << "\x1b[31m";
                else if (curColor==WHITE)
                    output << "\x1b[32m";

                output << curColor;
                
                if (curColor==RED || curColor==WHITE)
                    output << "\x1b[0m";
                    
                output << " ";
            }
            output << endl;
        }
        return output;            
    }
    void printBoardPaths(vector<vector<pair<int,int>>> paths) {
        unordered_map<pair<int,int>,string,pair_hash> pairToColor;
        for (int paths_index = 0; paths_index < paths.size(); ++paths_index) {
            auto path = paths[paths_index];
            for (int pair_index = 1; pair_index < path.size(); ++pair_index) {
                auto pair = path[pair_index];
                pairToColor[pair] = PATH_COLORS[paths_index];
            }
        }
        cout << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i)
            cout << " " << char('0'+i+1) << " ";
        cout << endl << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i)
            cout << "---";
        cout << endl;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            cout << char('A'+i) << " |";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                Color curColor = getPiece(i,j).getColor();
                cout << " ";

                if (pairToColor.find(make_pair(i,j))!=pairToColor.end()) {
                    cout << pairToColor[make_pair(i,j)] << "X";
                } else {
                    if (curColor==RED)
                        cout << "\x1b[31m";
                    else if (curColor==WHITE)
                        cout << "\x1b[32m";

                    cout << curColor;
                }
                cout << "\x1b[0m ";
            }
            cout << endl;
        }        
    }
    pair<int,int> getIndexFromLocation(string location) {
        int row = location[0]-'A';
        int col = location[1]-'1';
        return make_pair(row,col);
    }
    string getLocationFromIndex(pair<int,int> index) {
        char row = 'A'+index.first;
        char col = '1'+index.second;
        return {row,col};
    }
    void movePiece(pair<int,int> startLocation, pair<int,int> destLocation) {
        board[destLocation.first][destLocation.second] = board[startLocation.first][startLocation.second];
        Color pieceColor = getPiece(startLocation.first,startLocation.second).getColor();
        if ((pieceColor==RED && destLocation.first==0) || (pieceColor==WHITE && destLocation.first==BOARD_SIZE-1)) {
            board[destLocation.first][destLocation.second].setKing();
        }
        board[startLocation.first][startLocation.second].remove();
    }
    void executePath(vector<pair<int,int>> path) {
        pair<int,int> curLocation = path[0];
        for (int i = 1; i < path.size(); ++i) {
            if (abs(curLocation.first - path[i].first)==2) {
                board[(curLocation.first+path[i].first)/2][(curLocation.second+path[i].second)/2].remove();
            }
            movePiece(curLocation,path[i]);
            curLocation = path[i];
        }
    }

private:
    void dfs(pair<int,int> curNode, vector<pair<int,int>> curPath, vector<vector<pair<int,int>>>& paths, JUMP_TYPE jump_type, Piece curPiece) {
        if (curPiece.getColor() == NONE) return;

        // Push curNode into curPath
        curPath.push_back(curNode);

        vector<pair<int,int>> neighbors;
        // If (NONE || CAPTURE) && (can move +2|-2,+2|-2), type=capture
        if (jump_type == JUMP_NONE || jump_type == JUMP_CAPTURE) {
            if (isValidMove(curNode,curNode.first+2,curNode.second+2,curPiece)) {
                neighbors.push_back(make_pair(curNode.first+2,curNode.second+2));
                jump_type = JUMP_CAPTURE;
            }
            if (isValidMove(curNode,curNode.first+2,curNode.second-2,curPiece)) {
                neighbors.push_back(make_pair(curNode.first+2,curNode.second-2));
                jump_type = JUMP_CAPTURE;
            }
            if (isValidMove(curNode,curNode.first-2,curNode.second+2,curPiece)) {
                neighbors.push_back(make_pair(curNode.first-2,curNode.second+2));
                jump_type = JUMP_CAPTURE;
            }
            if (isValidMove(curNode,curNode.first-2,curNode.second-2,curPiece)) {
                neighbors.push_back(make_pair(curNode.first-2,curNode.second-2));
                jump_type = JUMP_CAPTURE;
            }
        }
        // If (NONE || NORMAL) && can move +1|-1,+1|-1, type=normal
        if (jump_type == JUMP_NONE) {
            if (isValidMove(curNode,curNode.first+1,curNode.second+1,curPiece)) {
                neighbors.push_back(make_pair(curNode.first+1,curNode.second+1));
                jump_type = JUMP_NORMAL;
            }
            if (isValidMove(curNode,curNode.first+1,curNode.second-1,curPiece)) {
                neighbors.push_back(make_pair(curNode.first+1,curNode.second-1));
                jump_type = JUMP_NORMAL;
            }
            if (isValidMove(curNode,curNode.first-1,curNode.second+1,curPiece)) {
                neighbors.push_back(make_pair(curNode.first-1,curNode.second+1));
                jump_type = JUMP_NORMAL;
            }
            if (isValidMove(curNode,curNode.first-1,curNode.second-1,curPiece)) {
                neighbors.push_back(make_pair(curNode.first-1,curNode.second-1));
                jump_type = JUMP_NORMAL;
            }
        }

        // if no neighbors, push curPath into paths
        if (neighbors.size()==0 && curPath.size() > 1)
            paths.push_back(curPath);

        // for each neighbor, call dfs
        for (pair<int,int> neighbor : neighbors)
            dfs(neighbor, curPath, paths, jump_type, curPiece);
    }
    
    Piece board[BOARD_SIZE][BOARD_SIZE];
};

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
            cout << "Player " << curColor << ", enter the location of the piece you would like to move (example: A4): ";
            cin >> selectedPieceLocation;
            if (board.validPiece(curColor, selectedPieceLocation))
                paths = board.generatePaths(selectedPieceLocation);
        } while (paths.size() == 0);

        // Prompt user for the path they would like to move the piece thorugh
        cout << "Here are the paths you can move through:" << endl;
        for (int i = 0; i < paths.size(); ++i) {
            cout << PATH_COLORS[i] << "  " << i << ": ";
            for (int j = 0; j < paths[i].size(); ++j) {
                cout << board.getLocationFromIndex(paths[i][j]) << " ";
            } 
            cout << "\x1b[0m " << endl;
        }
        cout << "===========================" << endl;
        board.printBoardPaths(paths);
        int selectedPath;
        do {
            cout << "Enter the number of path you would you like to move through (example: 1): ";
            cin >> selectedPath;
        } while (selectedPath < 0 || paths.size() <= selectedPath);

        // Execute the selected move
        board.executePath(paths[selectedPath]);

        // Switch players
        pair<int,int> curPieceIndex = paths[selectedPath][paths[selectedPath].size()-1];
        curColor = board.getPiece(curPieceIndex.first, curPieceIndex.second).getOppositeColor();
    }

    Color curColor;
    Board board;
};