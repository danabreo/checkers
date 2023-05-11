// Author: Daniel Abreo
// Test: g++ -std=c++14 test.cpp -lgtest -lgtest_main && ./a.out

#include "game.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

TEST(BoardConstructor, VerifyPossibleMoves) {
    Board board;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            auto location = board.getLocationFromIndex(make_pair(i,j));
            auto paths = board.generatePaths(location);
            auto color = board.getPiece(i,j).getColor();
            if (i<2 && (i+j)%2==1) {
                // non movable, white
                EXPECT_EQ(paths.size(),0);
                EXPECT_EQ(color, WHITE);
            } else if (i==2 && j==7) {
                // movable, white, edge case
                EXPECT_EQ(paths.size(),1);
                EXPECT_EQ(color, WHITE);
            } else if (i==2 && (i+j)%2==1) {
                // movable, white
                EXPECT_EQ(paths.size(),2);
                EXPECT_EQ(color, WHITE);
            } else if (i==5 && j==0) {
                // movable, red, edge case
                EXPECT_EQ(paths.size(),1);
                EXPECT_EQ(color, RED);
            } else if (i==5 && (i+j)%2==1) {
                 // movable, red
                EXPECT_EQ(paths.size(),2);
                EXPECT_EQ(color, RED);
            } else if (i>5 && (i+j)%2==1) {
                 // non movable, red
                EXPECT_EQ(paths.size(),0);
                EXPECT_EQ(color, RED);
            } else {
                EXPECT_EQ(paths.size(),0);
                EXPECT_EQ(color, NONE);
            }
        }
    }
}

TEST(HelperFunctions, ConvertIndexAndLocation) {
    vector<pair<int,int>> indexes = {{0,0},{3,4},{7,7}};
    vector<string> locations = {"A1","D5","H8"};
    
    Board board;

    for (int i = 0; i < indexes.size(); ++i) {
        auto expect_1 = indexes[i];
        auto result_1 = board.getIndexFromLocation(locations[i]);
        EXPECT_EQ(expect_1,result_1);

        auto expect_2 = board.getLocationFromIndex(indexes[i]);
        auto result_2 = locations[i];
        EXPECT_EQ(expect_2,result_2);
    }
}

TEST(BasicGame, ThreeTurnsEach) {
    Board board;
    board.executePath({{5,2},{4,3}});
    board.executePath({{2,5},{3,4}});
    auto paths = board.generatePaths("E4");
    EXPECT_THAT(paths,ElementsAre(ElementsAre(Pair(4,3),Pair(2,5))));

    EXPECT_EQ(board.getPiece(3,4).getColor(),WHITE);
    board.executePath({{4,3},{2,5}});
    EXPECT_EQ(board.getPiece(3,4).getColor(),NONE);

    EXPECT_EQ(board.getPiece(2,5).getColor(),RED);
    board.executePath({{1,4},{3,6}});
    EXPECT_EQ(board.getPiece(2,5).getColor(),NONE);

    board.executePath({{5,4},{4,5}});
    paths = board.generatePaths("D7");
    EXPECT_THAT(paths,ElementsAre(ElementsAre(Pair(3,6),Pair(5,4))));
    
    EXPECT_EQ(board.getPiece(4,5).getColor(),RED);
    board.executePath({{3,6},{5,4}});
    EXPECT_EQ(board.getPiece(4,5).getColor(),NONE);

    paths = board.generatePaths("G4");
    EXPECT_THAT(paths,ElementsAre(ElementsAre(Pair(6,3),Pair(4,5))));

    EXPECT_EQ(board.getPiece(5,4).getColor(),WHITE);
    board.executePath({{6,3},{4,5}});
    EXPECT_EQ(board.getPiece(5,4).getColor(),NONE);

    paths = board.generatePaths("C8");
    EXPECT_THAT(paths,ElementsAre(ElementsAre(Pair(2,7),Pair(3,6))));
    board.executePath({{2,7},{3,6}});
    board.executePath({{2,1},{3,0}});
    board.executePath({{7,4},{6,3}});
    board.executePath({{0,5},{1,4}});
    paths = board.generatePaths("E6");
    EXPECT_THAT(paths,ElementsAre(ElementsAre(Pair(4,5),Pair(2,7),Pair(0,5))));

    EXPECT_EQ(board.getPiece(3,6).getColor(),WHITE);
    EXPECT_EQ(board.getPiece(1,6).getColor(),WHITE);
    board.executePath({{4,5},{2,7},{0,5}});
    EXPECT_EQ(board.getPiece(3,6).getColor(),NONE);
    EXPECT_EQ(board.getPiece(1,6).getColor(),NONE);

    paths = board.generatePaths("A6");
    EXPECT_THAT(paths,ElementsAre(ElementsAre(Pair(0,5),Pair(1,6))));

}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}