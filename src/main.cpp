#define MAXTRIES 10;

#include <iostream>
#include <Labyrinths.hpp>
#include <set>
int main(int, char **)
{
    const std::string fileName("test.txt");
    const std::string blackEmoji("⬛");
    const std::string whiteEmoji("⬜");

    Lab::Labyrinth2D inputVector = Lab::Labyrinth2DGenerator::generateLabyrinth(Lab::Pattern::random, 10, 10);
    Lab::WaysVector resultWays = Lab::LabyrinthSolver::findAllWays(inputVector);
    Lab::Way foundPath{};
    int maxtries = MAXTRIES;
    // for (int i = 0; i < maxtries; i++){
    //     inputVector = Lab::Labyrinth2DGenerator::generateLabyrinth(Lab::Pattern::random, 100, 100);
    //     resultWays = Lab::LabyrinthSolver::findAllWays(inputVector);
    //     for (Lab::Way path : resultWays){
    //         if (path.size() > 10){
    //             foundPath = path;
    //         }
    //     }
    // }
    Lab::Labyrinth2D fixedLabyrinth = Lab::Labyrinth2DGenerator::connectSomeShapes(inputVector);
    
    Lab::PrintInterface::print2DContainer(fixedLabyrinth.blockField);
    Lab::PrintInterface::print2DLabyrinthToFile(fileName,inputVector.blockField, fixedLabyrinth.blockField, blackEmoji, whiteEmoji);
    // for (Lab::Position2D pos : foundPath){
    //     std::cout << pos.xPosition << " " << pos.yPosition << "\n";
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