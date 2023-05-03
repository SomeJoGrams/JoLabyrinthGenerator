

// TODO write GTESTs

// Lab::Way curWay{Lab::Position2D{0,0},Lab::Position2D{0,1},
//                                     Lab::Position2D{0,2},Lab::Position2D{1,0},Lab::Position2D{2,0},Lab::Position2D{2,1},
//                                     Lab::Position2D{2,2}, Lab::Position2D{1,2}};

// bool hasLoops = Lab::LabyrinthSolver::hasLoops(curWay);


#include "gtest/gtest.h"

#include "Labyrinths.hpp"


// TEST(TestSuite, TestName){

namespace Lab{
// The fixture for testing class Foo.
// alternative fixture for testing
class Position2DTest : public testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  Position2DTest() {
    // You can do set-up work for each test here.
  }

  ~Position2DTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
    //myVec[0] = 3;
    // firstPos{1,1};
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
    Lab::Position2D firstPos;
    Lab::Position2D secondPos;

};

TEST(Position2DTest, PosEqualOperation){
  Lab::Position2D firstPos{1,1};
  Lab::Position2D secondPos{1,1};
  EXPECT_EQ(firstPos, secondPos); 
}

TEST(Position2DTest, PosEqualOperationWrong){
  Lab::Position2D firstPos{1,1};
  Lab::Position2D secondPos{2,1};
  EXPECT_EQ(firstPos, secondPos) << "failed here"; 
}


// TODO use the fixture

}


int main(int argc, char **argv) { // not needed bc of including Gtest main
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// Lab::Labyrinth2D lab = Lab::Labyrinth2DGenerator::generateLabyrinth(Lab::Pattern::nothing,10,10);   
//  Lab::Labyrinth2D lab = Lab::Labyrinth2DGenerator::generateLabyrinth(Lab::Pattern::nothing,10,10);   
//     lab.blockField[3][3] = 0;
//     lab.blockField[4][3] = 0;
//     lab.blockField[5][3] = 0;
//     lab.blockField[3][4] = 0;
//     lab.blockField[3][5] = 0;
//     lab.blockField[4][5] = 0;
//     lab.blockField[5][5] = 0;