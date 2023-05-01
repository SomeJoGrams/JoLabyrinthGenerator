#define MAXTRIES 10;

#include <iostream>
#include <set>

#include "Labyrinths.hpp"

int main(int, char **)
{
    const std::string fileName("test.txt");
    const std::string blackEmoji("⬛");
    const std::string whiteEmoji("⬜");


    Lab::Labyrinth2D inputVector = Lab::Labyrinth2DGenerator::generateLabyrinth(Lab::Pattern::random, 15,15);
    
    //Lab::WaysVector resultWays = Lab::LabyrinthSolver::findAllWays(inputVector);

//  Lab::Way curWay{Lab::Position2D{0,0},Lab::Position2D{0,1},
//                                      Lab::Position2D{0,2},
//                                     //  Lab::Position2D{0,3},
//                                      Lab::Position2D{1,0},
//                                      Lab::Position2D{2,0},
//                                      Lab::Position2D{2,1},
//                                      Lab::Position2D{2,2}, 
//                                      Lab::Position2D{1,2}};

    //Lab::Way test = Lab::LabyrinthSolver::findConnectedTilesSet(lab);
    // for (const Lab::Position2D pos : test){
    //     std::cout << pos <<"\n";
    // }
    //Lab::PrintInterface::print2DContainer(lab.blockField);



    // int ind = 0;
    // std::set<Lab::Position2D> replacePositions{Lab::Position2D{2,0}};
    // //std::vector<Lab::Position2D> positionCandidates{Lab::Position2D{2,0},Lab::Position2D{2,1}};
    // std::vector<Lab::Position2D> positionCandidates{Lab::Position2D{2,1}};

    // while (replacePositions.find(positionCandidates[ind]) != replacePositions.end()){ 
    //     ind+=1;
    // }
    // std::cout << ind << "test\n";

    // bool hasLoops = Lab::LabyrinthSolver::hasLoops(curWay);
    // std::cout << "hasLoops?" << hasLoops << "n";

    //Lab::Labyrinth2D fixedLabyrinth = Lab::Labyrinth2DGenerator::connectSomeShapes(inputVector);
    Lab::Labyrinth2D fixedLabyrinth = Lab::Labyrinth2DGenerator::connectAllShapes(inputVector);
    

    Lab::PrintInterface::print2DContainer(fixedLabyrinth.blockField);
    Lab::PrintInterface::print2DLabyrinthToFile(fileName,inputVector.blockField, fixedLabyrinth.blockField, blackEmoji, whiteEmoji,false);
   
   
   
    // Lab::PrintInterface::print2DContainer(inputVector.blockField);
    // Lab::PrintInterface::print2DLabyrinthToFile(fileName,inputVector.blockField, fixedLabyrinth.blockField, blackEmoji, whiteEmoji,true);
    
    // int wayInd = 0;
    // for (Lab::Way way : resultWays){
    //     std::cout << "current Way:" << wayInd << "\n";
    //     for (Lab::Position2D pos : way){
    //         std::cout << "x" << pos.xPosition << " y" << pos.yPosition << "\n";
    //     }
    //     wayInd += 1;
    // }
    
    // Lab::PrintInterface::print2DContainer(inputVector.blockField);
    // std::cout << tries;
    
    
    //std::cout << x << " " << "\n"; // ⬛


    //inputVector.blockField = Lab::Labyrinth2DGenerator::fillLines(inputVector.blockField, Lab::Position2D{3,2}, Lab::Position2D{5,4});
    //Lab::PrintInterface::print2DContainer(inputVector.blockField);
    //Lab::Position2D x = Lab::Position2D{2,3};
    return 0;

}



    // // short erase test -> deleted vector element stays
    // std::vector<int> vect{1,2,3,4,5,6};
    // for (int x : vect){
    //     std::cout << x << "\n";
    // }
    // int firstElement = vect.front();
    // std::cout << "first element in vector";
    // std::cout << firstElement << "\n";
    // vect.erase(vect.begin());
    // std::cout << "deleting first element in vector \n";
    // std::cout << firstElement << "\n";
    // std::cout << "vector content \n";
    // for (int x : vect){
    //     std::cout << x << "\n";
    // }

        // find seq
    // std::vector<int> vect = {1,2,3,4,5,6,7,8,9};
    // auto result = std::find(vect.begin(), vect.end(), 4);
    // if (result == vect.end()){
    //     std::cout << "not found";
    // }
    // vect.erase(result + 1, vect.end());
    // for (int x : vect){
    //     std::cout << x << "\n";
    // }