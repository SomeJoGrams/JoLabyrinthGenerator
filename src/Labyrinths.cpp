#include <iostream>
#include <cmath>
#include <random>
#include <bitset>
#include <algorithm>
#include <set>
#include <map>
#include <fstream>
#include <string>


#include "Labyrinths.hpp"

namespace Lab
{

    /*
* return 3 if same Point
* return 2 if same x Coord
* return 1 if same y Coord
* return 0 if not on Same line
*/

    inline TakenPaths takePath(const Position2D fromPos, const Position2D toPos,const TakenPaths takenPaths){
        TakenPaths resultPath(takenPaths);
        int posDif = PositionInterface::adjacentPoint(fromPos, toPos);
        if (posDif == 1){
            resultPath.rightPath = true; // TODO error hier
        }
        else if (posDif == 2){
            resultPath.bottomPath = true;
        }
        else if (posDif == 3){
            resultPath.leftPath = true;
        }
        else if (posDif == 4){
            resultPath.topPath = true;
        }
        // else{
            
        // }
        return resultPath;
    }

    inline bool pathAlreadyTaken(const Position2D fromPos, const Position2D toPos,const TakenPaths takenPaths){
        TakenPaths resultPath(takenPaths);
        int posDif = PositionInterface::adjacentPoint(fromPos, toPos);
        if (posDif == 1){
            return resultPath.rightPath == takenPaths.rightPath;
        }
        else if (posDif == 2){
            return resultPath.bottomPath == takenPaths.bottomPath;
        }
        else if (posDif == 3){
            return resultPath.leftPath == takenPaths.leftPath;
        }
        else if (posDif == 4){
            return resultPath.topPath == takenPaths.topPath;
        }
        return false;
    }

    /*
    * calculate the relative position between fromPos and toPos and check whether the takenPaths already contain 
    a different path 
    */
    inline bool differentPathAlreadyTaken(const Position2D fromPos, const Position2D toPos,const TakenPaths takenPaths){
        TakenPaths resultPath(takenPaths);
        int posDif = PositionInterface::adjacentPoint(fromPos, toPos);
        if (posDif == 1){
            //resultPath.rightPath == takenPaths.rightPath;
            return resultPath.bottomPath || resultPath.leftPath || resultPath.topPath;
        }
        else if (posDif == 2){
            //resultPath.bottomPath == takenPaths.bottomPath;
            return resultPath.topPath || resultPath.rightPath || resultPath.leftPath;
        }
        else if (posDif == 3){
            //resultPath.leftPath == takenPaths.leftPath;
            return resultPath.topPath || resultPath.rightPath || resultPath.bottomPath;
        }
        else if (posDif == 4){
            //resultPath.topPath == takenPaths.topPath;
            return resultPath.rightPath || resultPath.leftPath || resultPath.bottomPath;
        }
        return false;
    }

    inline TakenPaths combinePaths(const TakenPaths path1, const TakenPaths path2){
        TakenPaths resultPath{false,false,false,false};
        resultPath.rightPath = path1.rightPath || path2.rightPath;
        resultPath.leftPath = path1.leftPath || path2.leftPath;
        resultPath.bottomPath = path1.bottomPath || path2.bottomPath;
        resultPath.topPath = path1.topPath || path2.topPath;
        return resultPath;
    }


    int PositionInterface::onSameLine(const Position2D firstPos, const Position2D secondPos)
    { // make int for same y / x Position
        bool xCoord = firstPos.xPosition == secondPos.xPosition;
        bool yCoord = firstPos.yPosition == secondPos.yPosition;
        if (xCoord && yCoord)
        {
            return 3; // if same point
        }
        else if (xCoord)
        {
            return 2; // if on same x Coordinate
        }
        else if (yCoord)
        {
            return 1; // on same y Coordinate
        }
        return 0; // not related
    }

    /*
* start and end of Points inclusive
*/
    bool PositionInterface::betweenTwoPoints(const Position2D point, const Position2D firstPos, const Position2D secondPos)
    { // make check for first and second Pos reversed
        // check space between firstPos and secondPos -> that could allow to draw rectangles later on
        auto maxX = std::max(firstPos.xPosition, secondPos.xPosition);
        auto maxY = std::max(firstPos.yPosition, secondPos.yPosition);
        auto minX = std::min(firstPos.xPosition, secondPos.xPosition);
        auto minY = std::min(firstPos.yPosition, secondPos.yPosition);
        return (point.xPosition >= minX && point.xPosition <= maxX) && (point.yPosition >= minY && point.yPosition <= maxY);
    }

    // return the natural distance of 2 Points only going straight up/down and left/right
    long long int PositionInterface::pointDistance(const Position2D point1, const Position2D point2)
    {
        auto greaterXCoord = std::max(point1.xPosition, point2.xPosition);
        auto greaterYCoord = std::max(point1.yPosition, point2.yPosition);
        auto smallerXCoord = std::min(point1.xPosition, point2.xPosition);
        auto smallerYCoord = std::min(point1.yPosition, point2.yPosition);
        //example p1 (1,2), p2 (2,4)
        // 4-2 = 2
        // 2-1 = 1
        // distance 3
        return (greaterYCoord - smallerYCoord) + (greaterXCoord - smallerXCoord);
    }

    /*
*   only right input if first pos and second pos are on same y or x Coord ( a line)
*/
    std::vector<Position2D> PositionInterface::enclosedPoints(const bool xPos, const Position2D firstPos, const Position2D secondPos)
    {
        std::vector<Position2D> resultVector;
        const auto maxX = std::max(firstPos.xPosition, secondPos.xPosition);
        const auto maxY = std::max(firstPos.yPosition, secondPos.yPosition);
        auto curX = std::min(firstPos.xPosition, secondPos.xPosition);
        auto curY = std::min(firstPos.yPosition, secondPos.yPosition);
        if (xPos)
        {
            for (auto xCoord = curX; xCoord <= maxX; xCoord++)
            {
                resultVector.push_back(Position2D{xCoord, curY});
            }
        }
        else
        {
            for (auto yCoord = curY; yCoord <= maxY; yCoord++)
            {
                resultVector.push_back(Position2D{yCoord, curY});
            }
        }
        return resultVector;
    }

    Position2D PositionInterface::nearestPoint(const Position2D point, const Way severalPoints)
    {
        Position2D currentResultPoint = severalPoints[0];
        long long int minDistance = LONG_MAX; // TODO LONG LONG MAX
        long long int currentDistance = LONG_MAX;
        for(Position2D comparePoint : severalPoints){
            currentDistance = pointDistance(point, comparePoint);
            if (currentDistance < minDistance){
                minDistance = currentDistance;
                currentResultPoint = comparePoint;
            }
        }
        return currentResultPoint;
    }

/*
*   check that the point2 is not diagonal from the point1, but has one distance
*   X 4 X
*   3 O 1  
*   X 2 X
*/
    int PositionInterface::adjacentPoint(const Position2D origin, const Position2D goal){
        if (origin.xPosition + 1== goal.xPosition && origin.yPosition == goal.yPosition){
            return 1;
        }
        else if(origin.xPosition == goal.xPosition && origin.yPosition - 1 == goal.yPosition){
            return 2;
        }
        else if(origin.xPosition - 1== goal.xPosition && origin.yPosition == goal.yPosition){
            return 3;
        }
        else if(origin.xPosition == goal.xPosition && origin.yPosition + 1 == goal.yPosition){
            return 4;
        }         
        return 0;
        
    }
    
    bool PositionInterface::adjacentToWay(const Position2D point, const Way severalPoints){
        for (const Position2D pos : severalPoints){
            if (adjacentPoint(point, pos) != 0){
                return true;
            }
        }
        return false;
    }
    
    int PositionInterface::timesAdjacentToWay(const Position2D point, const Way severalPoints){
        int result(0);
        for (const Position2D pos : severalPoints){
            if (adjacentPoint(point, pos) != 0){
                result += 1;
            }
        }
        return result;
    }


    BlockField2D Labyrinth2DGenerator::generateEmptyBlockfield(const int xSize, const int ySize)
    {
        BlockField2D newBlockField = {};
        // std::vector<bool> blockVector(ySize);
        for (int xIndex = 0; xIndex < xSize; xIndex++)
        {
            newBlockField.emplace_back(ySize);
        }
        return newBlockField;
    }

    Labyrinth2D Labyrinth2DGenerator::generateLabyrinth(const Pattern generationPattern, const int xSize, const int ySize)
    { // maybe change to type long
        int ySizeFixed = ySize + 1;
        BlockField2D newBlockField = generateEmptyBlockfield(xSize, ySize);
        // fill border
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{0, 0}, Position2D{static_cast<long long int>(xSize), 0}, static_cast<bool>(1));
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{0, 0}, Position2D{0, static_cast<long long int>(ySizeFixed)}, static_cast<bool>(1));
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{static_cast<long long int>(xSize-1), 0}, Position2D{static_cast<long long int>(xSize-1), static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{0, static_cast<long long int>(ySizeFixed-1-1)}, Position2D{static_cast<long long int>(xSize-1), static_cast<long long int>(ySizeFixed-1-1)}, static_cast<bool>(1));
        // fill border end
        Position2D startPos = {1, 5};
        Position2D endPos = {xSize - 2, 1};
        switch (generationPattern){
            case Pattern::empty: // rename this bc there are lines
                // fill middle Square
                //newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{2, 2}, Position2D{7, 7}, static_cast<bool>(1));
                //fill middle square end
                // fill lines
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{2, 0}, Position2D{2, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{4, 0}, Position2D{4, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{6, 0}, Position2D{6, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{8, 0}, Position2D{8, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                // fill lines end

                return Labyrinth2D{startPos, endPos, newBlockField};
            case Pattern::nothing:
                // TODO put the line generation in for loops, fill every second line
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{1, 0}, Position2D{1, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));                
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{2, 0}, Position2D{2, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{3, 0}, Position2D{3, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{4, 0}, Position2D{4, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{5, 0}, Position2D{5, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{6, 0}, Position2D{6, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{7, 0}, Position2D{7, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{8, 0}, Position2D{8, static_cast<long long int>(ySizeFixed-1)}, static_cast<bool>(1));
                // fill lines end
                Position2D startPos = {3, 3};
                Position2D endPos = {xSize - 2, 1};
                return Labyrinth2D{startPos, endPos, newBlockField};
                break;
            // case Pattern::insideOut:
            //     break;
        }
        newBlockField = Labyrinth2DGenerator::patternGeneration(newBlockField, generationPattern);
        startPos = {1, 1};
        endPos = {2, 2};
        return Labyrinth2D{startPos, endPos, newBlockField};
    };

    BlockField2D Labyrinth2DGenerator::patternGeneration(BlockField2D blockField, const Pattern generationPattern)
    {
        Lab::RandomSteps randStep{1, -1, 0, 49};
        Lab::RandomWalk randWalk{randStep};
        std::vector<std::vector<int>> rwField = randomWalk(blockField.size(), blockField[0].size(), 3, 100, randWalk);// generate the random walk field
        blockField = firstLabAlgorithm(rwField);
        return blockField;
    }

    BlockField2D Labyrinth2DGenerator::firstLabAlgorithm(std::vector<std::vector<int>> rWBlockField)
    {
        // generate empty field
        BlockField2D newBlockField = generateEmptyBlockfield(rWBlockField.size(), rWBlockField[0].size());
        for (size_t xCoord = 0; xCoord < rWBlockField.size(); xCoord += 2)
        {
            if (xCoord <= rWBlockField.size() - 1)
            {
                newBlockField = fillLines(newBlockField, Position2D{static_cast<long long int>(xCoord), 0},
                                          Position2D{static_cast<long long int>(xCoord), static_cast<long long int>(rWBlockField[0].size() - 1)}, 1);
            }
        }
        for (size_t yCoord = 0; yCoord < rWBlockField[0].size(); yCoord += 2)
        {
            if (yCoord <= rWBlockField[0].size() - 1)
            {
                newBlockField = fillLines(newBlockField, Position2D{0, static_cast<long long int>(yCoord)},
                                          Position2D{static_cast<long long int>(rWBlockField.size() - 1), static_cast<long long int>(yCoord)}, 1);
            }
        }

        // look at blocks surrounding the field, fill with 1 if number is high enough and fill other numbers on same x with wall too?
        for (auto vecIter = rWBlockField.begin(); vecIter != rWBlockField.end(); vecIter++)
        {
            auto indexXCoord = vecIter - rWBlockField.begin();
            for (auto bitIter = (*vecIter).begin(); bitIter != (*vecIter).end(); bitIter++)
            {
                auto indexYCoord = bitIter - (*vecIter).begin();
                bool inBounds = indexXCoord > 0 && indexYCoord > 0 && (indexXCoord < rWBlockField.size() - 1 && indexYCoord < rWBlockField[0].size() - 1);
                auto startValue = rWBlockField[indexXCoord][indexYCoord];
                bool sameValues = true;
                if (startValue > 1 && inBounds && newBlockField[indexXCoord][indexYCoord] == 1 && newBlockField[indexXCoord - 1][indexYCoord] == 0 && newBlockField[indexXCoord + 1][indexYCoord] == 0) //left to right way
                {
                    // open way if all points between the left and the right zero have the same random walk number
                    std::vector<Position2D> pointVec = PositionInterface::enclosedPoints(true, Position2D{indexXCoord - 1, indexYCoord}, Position2D{indexXCoord + 1, indexYCoord});
                    for (auto point : pointVec)
                    {
                        if (rWBlockField[point.xPosition][point.yPosition] != startValue)
                        {
                            sameValues = false;
                        }
                    }
                    if (sameValues)
                    {
                        newBlockField[indexXCoord][indexYCoord] = 0;
                    }
                }
                if (startValue > 1 && inBounds && newBlockField[indexXCoord][indexYCoord] == 1 && newBlockField[indexXCoord][indexYCoord - 1] == 0 && newBlockField[indexXCoord][indexYCoord + 1] == 0) //up down way
                {
                    std::vector<Position2D> pointVec = PositionInterface::enclosedPoints(false, Position2D{indexXCoord, indexYCoord - 1}, Position2D{indexXCoord, indexYCoord + 1});
                    for (auto point : pointVec)
                    {
                        if (rWBlockField[point.xPosition][point.yPosition] != startValue)
                        {
                            sameValues = false;
                        }
                    }
                    if (sameValues)
                    {
                        newBlockField[indexXCoord][indexYCoord] = 0;
                    }
                }
            }
        }
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{0, 0}, Position2D{static_cast<long long int>(rWBlockField.size()), 0}, static_cast<bool>(1));
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{0, 0}, Position2D{0, static_cast<long long int>(rWBlockField[0].size())}, static_cast<bool>(1));
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{static_cast<long long int>(rWBlockField.size()-1), 0}, Position2D{static_cast<long long int>(rWBlockField.size()-1), static_cast<long long int>(rWBlockField[0].size()-1)}, static_cast<bool>(1));
        newBlockField = Labyrinth2DGenerator::fillLines(newBlockField, Position2D{0, static_cast<long long int>(rWBlockField[0].size()-1)}, Position2D{static_cast<long long int>(rWBlockField.size()), static_cast<long long int>(rWBlockField[0].size()-1)}, static_cast<bool>(1));
        return newBlockField;
    }
    /*
* fills from one point to another if they share a coordiante (rangeChecked?)
*/
    BlockField2D Labyrinth2DGenerator::fillLines(BlockField2D blockField, const Position2D pos1, const Position2D pos2, const bool filling)
    { // TODO add bool operation
        // check Coodinates on line
        for (auto vecIter = blockField.begin(); vecIter != blockField.end(); vecIter++)
        {
            auto indexXCoord = vecIter - blockField.begin();
            for (auto bitIter = (*vecIter).begin(); bitIter != (*vecIter).end(); bitIter++)
            {
                auto indexYCoord = bitIter - (*vecIter).begin();
                // check one same line like pos1, pos2
                if (PositionInterface::betweenTwoPoints(Position2D{indexXCoord, indexYCoord}, pos1, pos2))
                {
                    blockField[indexXCoord][indexYCoord] = filling;
                }
            }
        }
        return blockField;
    }
    
    BlockField2D Labyrinth2DGenerator::fillPoints(const BlockField2D blockField,const Way pointsToFill, const bool filling){
        BlockField2D copiedBlockField = blockField;
        for (Position2D point : pointsToFill){
            copiedBlockField[point.xPosition][point.yPosition] = filling;
        }
        return copiedBlockField;
    }

    // 1 en sind die Wände, random walk generator der von Mitte bis zum Rand gehen kann
    std::vector<std::vector<int>> Labyrinth2DGenerator::randomWalk(const int xSize, const int ySize,const int randomWalkCount,const int stepsPerRandomWalk,RandomWalk &randWalker)
    {
        // generate Int-Field with same size as the BlockField
        std::vector<std::vector<int>> randomField{};
        for (size_t i = 0; i < xSize; i++)
        {
            randomField.emplace_back(ySize); // puts xSize vectors to the vectors
        }

        std::random_device rd;
        std::mt19937_64 ranGenerator64{rd()};
        std::uniform_int_distribution<> distr(0, 1);

        const Position2D startPosition{xSize / 2, ySize / 2};
        for (size_t i = 2; i < randomWalkCount + 1; i++) // use 1 to mark Walls later
        {
            Position2D currentPosition = startPosition;
            for (size_t walkStep = 0; walkStep < stepsPerRandomWalk; walkStep++)
            {
                if (distr(ranGenerator64) == 1)
                { // random walk on x coordinate
                    int xWalk = randWalker.generateStepWithinRange(currentPosition.xPosition, 0, xSize - 1);
                    currentPosition = Position2D{xWalk, currentPosition.yPosition};
                }
                else
                { // random walk on y coordinate
                    int yWalk = randWalker.generateStepWithinRange(currentPosition.yPosition, 0, ySize - 1);
                    currentPosition = Position2D{currentPosition.xPosition, yWalk};
                }
                randomField[currentPosition.xPosition][currentPosition.yPosition] = i;
            }
        }
        return randomField;
    }

    Labyrinth2D Labyrinth2DGenerator::connectSomeShapes(const Labyrinth2D lab2D){
        Labyrinth2D labCopy = lab2D;
        WaysVector freeConnectedTiles = LabyrinthSolver::findAllWays(lab2D);
        // sort by Size
        // 1. Version with Lambda
        std::sort(freeConnectedTiles.begin(), freeConnectedTiles.end(), [&](const Way& way1,const Way& way2 ){return way1.size() < way2.size();}); // TODO FIX LAMBDA
        // 2. Version with struct Functor
        // struct compSize{
        //     bool operator()(const Way& way1, const Way& way2){
        //         return way1.size() < way2.size();
        //     }
        // } smallerComparer;
        //std::sort(freeConnectedTiles.begin(), freeConnectedTiles.end(), smallerComparer); // TODO FIX LAMBDA

        // connect nearest tiles from smallest Tiles to greatest
        BlockField2D curBlockField = labCopy.blockField;
        int curIndex = 0;
        for (Way currentWay : freeConnectedTiles){
            // bool nearRightEnd = 
            // if (nearRightEnd){
            //     // connect to the left
            // }
            //else 
            if (currentWay.size() == 1){
                if (curIndex % 2 == 0){
                    curBlockField[currentWay[0].xPosition][currentWay[0].yPosition + 1] = 0;
                }
                else {
                    curBlockField[currentWay[0].xPosition + 1][currentWay[0].yPosition] = 0;
                }
                curIndex += 1;
            }
            else if (currentWay.size() >= 3){
                curBlockField[currentWay[0].xPosition][currentWay[0].yPosition + 1] = 0;
            }
        }
        labCopy.blockField = curBlockField;
        return labCopy;
    }

    Labyrinth2D Labyrinth2DGenerator::connectSomeShapes2(const Labyrinth2D lab2D){
        Labyrinth2D labCopy = lab2D;
        WaysVector freeConnectedTiles = LabyrinthSolver::findAllWays(lab2D);
        // sort by Size
        std::sort(freeConnectedTiles.begin(), freeConnectedTiles.end(), [&](const Way& way1,const Way& way2){return way1.size() < way2.size();});

        // connect nearest tiles from smallest Tiles to greatest
        BlockField2D curBlockField = labCopy.blockField;
        //Way alreadyConnectedWay = freeConnectedTiles[0];
        std::set<Position2D> replacePositions;
        //for (int i= 0; i < freeConnectedTiles.size() - 1; i++){
        std::random_device rd;
        std::mt19937_64 ranGenerator64{rd()};
        for (auto tilesIt = freeConnectedTiles.cbegin(); tilesIt != freeConnectedTiles.cend(); tilesIt++) // the tiles of the vector, a ways vector
        {
            // idea every way needs 1 piece except one way
            // IDEA connect randomly to a different way
            // not freeing pieces at the border, f.e down, right at the lower edge 
            // furthermore pieces that are already connecting other pieces(not existing walls) shouldnt be used
            // and already connected pieces have to be taken in account
            Way positionCandidates;
            std::cout << "current island\n";
            for (const Position2D pos : (*tilesIt)){
                if (pos.xPosition % 2 == 1 && pos.yPosition % 2 == 1){ // the rows and colums are only on uneven positions!
                    // extend the border by 1 to not take additional positions on the border
                    // Way neighbors = LabyrinthSolver::freeNeighbors(curBlockField,pos,1);
                    // TODO FIX TOFIX  Problem island can be connected producing a loop
                    Way neighbors = LabyrinthSolver::freeNeighborsDifferentIsland(curBlockField,(*tilesIt),pos,1);
                    positionCandidates.insert(positionCandidates.end(), neighbors.begin(), neighbors.end());
                    std::cout << "looked for neightbors for " << pos <<"\n";
                    for (auto pos1 : neighbors){
                        std::cout << "neighbour" << pos1 << "\n";
                    }
                    // look at all positionCandidates and only take a unique position
                }
            }
            std::uniform_int_distribution<> distr(0, positionCandidates.size() - 1);
            int ind = distr(ranGenerator64); // calculate a random start index
            // only insert unique positionCandidates, which connect to a new island
            int maxTries = positionCandidates.size() - 1;
            if (positionCandidates.size() != 0){
                // if the position is already inserted find a different one
                while (replacePositions.find(positionCandidates[ind]) != replacePositions.end()){ 
                    if (ind == positionCandidates.size() - 1){
                        ind = 0;
                    }
                    else{
                        ind += 1;
                    }
                    maxTries -= 1;
                    if (maxTries == 0){
                        break;
                    }
                }
                replacePositions.insert(positionCandidates[ind]);
            }
        }
        for (Position2D pos : replacePositions){
            std::cout << "taken position" << pos << "\n";
            curBlockField[pos.xPosition][pos.yPosition] = 0;
        }
            // if (currentWay.size() == 1){
            //     if (curIndex % 2 == 0){
            //         curBlockField[currentWay[0].xPosition][currentWay[0].yPosition + 1] = 0;
            //     }
            //     else {
            //         curBlockField[currentWay[0].xPosition + 1][currentWay[0].yPosition] = 0;
            //     }
            //     curIndex += 1;
            // }
            // else if (currentWay.size() >= 3){
            //     curBlockField[currentWay[0].xPosition][currentWay[0].yPosition + 1] = 0;
            // }
        
        labCopy.blockField = curBlockField;
        return labCopy;
    }

    Labyrinth2D Labyrinth2DGenerator::connectAllShapes(const Labyrinth2D lab2D){
        Labyrinth2D labCopy = lab2D;
        WaysVector freeConnectedTiles = LabyrinthSolver::findAllWays(lab2D);
        const int islandRepititions = freeConnectedTiles.size() - 1;
        BlockField2D curBlockField = labCopy.blockField;

        std::set<Position2D> replacePositions;

        std::random_device rd;
        std::mt19937_64 ranGenerator64{rd()};
        Way resultIsland = freeConnectedTiles[0];

        Way positionCandidates;
        // std::cout << "connected size" << freeConnectedTiles.size() << "\n";
        for (int islandConnection = 0; islandConnection <= islandRepititions; islandConnection++){
            // std::cout << "cur Run" << islandConnection << "\n";

            positionCandidates = Way();
            for (const Position2D pos : resultIsland){
                if (pos.xPosition % 2 == 1 && pos.yPosition % 2 == 1){ // the rows and colums are only on uneven positions!
                    // extend the border by 1 to not take additional positions on the border
                    Way neighbors = LabyrinthSolver::freeNeighborsDifferentIsland(curBlockField,resultIsland,pos,1);
                    positionCandidates.insert(positionCandidates.end(), neighbors.begin(), neighbors.end());
                    // look at all positionCandidates and only take a unique position
                }
            }
            std::uniform_int_distribution<> distr(0, positionCandidates.size() - 1);
            int ind = distr(ranGenerator64); // calculate a random start index
            // only insert unique positionCandidates, which connect to a new island
            int maxTries = positionCandidates.size() - 1;
            if (positionCandidates.size() != 0){
                // if the position is already inserted find a different one
                while (replacePositions.find(positionCandidates[ind]) != replacePositions.end()){ 
                    if (ind == positionCandidates.size() - 1){
                        ind = 0;
                    }
                    else{
                        ind += 1;
                    }
                    maxTries -= 1;
                    if (maxTries == 0){
                        break;
                    }
                }
                Position2D choosenCandidate = positionCandidates[ind];
                curBlockField[choosenCandidate.xPosition][choosenCandidate.yPosition] = 0;
            }
            freeConnectedTiles = LabyrinthSolver::findAllWays(curBlockField);// find all islands again // TODO remove this call 
            resultIsland = freeConnectedTiles[0];
        }
            
        labCopy.blockField = curBlockField;
        return labCopy;
        } 


    template <typename Val>
    void PrintInterface::print2DContainer(const std::vector<std::vector<Val>> &inputVector)
    {
        auto indexColumn = 0;
        // every inner vector itor has to have the same size
        for (auto yIndex = 0; yIndex < inputVector[0].size(); yIndex++)
        {
            for (auto xIndex = 0; xIndex < inputVector.size(); xIndex++)
            {
                // auto indexXCoord = vecIter - inputVector.begin();
                std::cout << inputVector[xIndex][yIndex]; // reverse x and y Index
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    void PrintInterface::print2DLabyrinthToFile(const std::string fileName,const std::vector<std::vector<bool>> &inputVector,const std::string freeFieldEmojiUnicode,const std::string notFreeEmojiUnicode)
    {
        // open the fileStream
        // convert to utf code by (\U+2B1B) 1. remove "+", 2. backslash before U 3. replace + with 3 zero
        //std::fstream fileStream(fileName, fileStream.trunc | fileStream.out);
        std::fstream fileStream(fileName, fileStream.trunc | fileStream.out);
        // stop print if file cant be opened
        if (!fileStream.is_open()){
            std::cout << "couldnt open the file";
            return;
        }
        // write stuff
        auto indexColumn = 0;
        // every inner vector itor has to have the same size
        for (auto yIndex = 0; yIndex < inputVector[0].size(); yIndex++)
        {
            for (auto xIndex = 0; xIndex < inputVector.size(); xIndex++)
            {
                if (inputVector[xIndex][yIndex]){ // its a free Field
                    fileStream << freeFieldEmojiUnicode;
                }
                else{
                    fileStream << notFreeEmojiUnicode;
                }
            }
            fileStream << '\n';
        }
        fileStream << '\n';
    }

    void PrintInterface::print2DLabyrinthToFile(const std::string fileName,
        const std::vector<std::vector<bool>> &inputVector, const std::vector<std::vector<bool>> &inputVector2,
        const std::string freeFieldEmojiUnicode,const std::string notFreeEmojiUnicode,
        const bool printIndices)
    {
        // open the fileStream
        // convert to utf code by (\U+2B1B) 1. remove "+", 2. backslash before U 3. replace + with 3 zero
        std::fstream fileStream(fileName, fileStream.trunc | fileStream.out);
        // stop print if file cant be opened
        if (!fileStream.is_open()){
            std::cout << "couldnt open the file";
            return;
        }
        // write stuff
        auto indexColumn = 0;
        // every inner vector itor has to have the same size
        if (printIndices){
            fileStream << "X";
            for (auto xIndex = 0; xIndex < inputVector.size(); xIndex++){
                fileStream << xIndex;
            }
            fileStream << "\n";
        }

        for (auto yIndex = 0; yIndex < inputVector[0].size(); yIndex++)
        {
            if (printIndices){
                fileStream << yIndex;
            }
            for (auto xIndex = 0; xIndex < inputVector.size(); xIndex++)
            {
                if (inputVector[xIndex][yIndex]){ // its a free Field
                    fileStream << freeFieldEmojiUnicode;
                }
                else{
                    fileStream << notFreeEmojiUnicode;
                }
            }
            fileStream << '\n';
        }
        fileStream << '\n';
        for (auto yIndex = 0; yIndex < inputVector2[0].size(); yIndex++)
        {
            for (auto xIndex = 0; xIndex < inputVector2.size(); xIndex++)
            {
                if (inputVector2[xIndex][yIndex]){ // its a free Field
                    fileStream << freeFieldEmojiUnicode;
                }
                else{
                    fileStream << notFreeEmojiUnicode;
                }
            }
            fileStream << '\n';
        }
        fileStream << '\n';
    }

    int RandomWalk::generateStep()
    {
        std::random_device rd;
        std::mt19937_64 ranGenerator64{rd()};
        std::uniform_int_distribution<> distr(0, 99);
        if (distr(ranGenerator64) >= randomStepObject.posValueAccept)
        { // checks if randomNumber is greater than the positiveValue, with posValueAccept == 99 -> no values accepted, with == 0 -> all values
            currentWalkPosition = currentWalkPosition + randomStepObject.posStepSize;
            return currentWalkPosition;
        }
        currentWalkPosition = currentWalkPosition + randomStepObject.negStepSize;
        return currentWalkPosition;
    }

    /*
* startRange need to be smaller than the EndRange
* only supports positive values
*/
    int RandomWalk::generateStepWithinRange(int startPosition, int startRange, int endRange)
    {
        std::random_device rd;
        std::mt19937_64 ranGenerator64{rd()};
        std::uniform_int_distribution<> distr(0, 99);
        int fitToRange(0);
        currentWalkPosition = startPosition;
        RandomWalk::generateStep(); // make step in RandomWalk Object, writes result to CurrentWalkPosition
        // fit the value to the range if too big or too low
        // bspw. currentWalkPosition = 1 und startRange beginn erst bei 10 und endet bei 15 -> sollte dann einfacher der erste Wert rauskommen? bei 2 der 2. usw.
        //-7 -6 -5 -4 -3 -2 -1 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
        //15 10 11 12 13 14 15 10 11 12 13 14 15 10 11 12 13 14 15 10 11 12 13
        int distance = endRange - startRange;
        if (currentWalkPosition >= 0)
        { // for postive start with 10 and add the modulo offset
            fitToRange = currentWalkPosition % distance;
            fitToRange += startRange;
        }
        else
        {
            int posWalkPosition = currentWalkPosition * -1; // calculate the positive value and start from the back
            fitToRange = endRange - ((posWalkPosition - 1) % distance);
        }
        return fitToRange;
    }

    RandomWalk::RandomWalk(RandomSteps randSteps)
        : randomStepObject(randSteps), currentWalkPosition{0}
    {
    }

    void RandomWalk::clearCurrentWalk()
    {
        RandomWalk::currentWalkPosition = 0;
    }

    // Labyrinth Solver,DFS
    Way LabyrinthSolver::findWay(const Labyrinth2D lb2D)
    {
        std::vector<Position2D> currentWayList{lb2D.startPosition};
        std::vector<Position2D> jumpBackStack;
        Position2D currentPosition{lb2D.startPosition};
        //std::vector<std::vector<bool>> visitedFields{lb2D.blockField.size() + lb2D.blockField[0].size()}; // FIX
        
        std::vector<bool> visitedFields(lb2D.blockField.size() * lb2D.blockField[0].size());
        // worst case run time of n*m
        
        visitedFields[currentPosition.xPosition + lb2D.blockField.size() * currentPosition.yPosition] = 1;
        for (auto vecIter = lb2D.blockField.begin(); vecIter != lb2D.blockField.end(); vecIter++)
        {
            for (auto bitIter = (*vecIter).begin(); bitIter != (*vecIter).end(); bitIter++)
            {
                //, 2.mark as visited,3. store jump back table if it still has free fields around
                //1. find next free field, that is not visited and free in the labyrinth
                // end if last Field is reached or there are no more ways
                bool viableWayFound = false;
                Position2D nextField{};
                if(currentPosition == lb2D.goalPosition){
                    return currentWayList;
                }
                // possibility to go right
                if (lb2D.blockField[currentPosition.xPosition + 1][currentPosition.yPosition] == 0 &&
                 visitedFields[currentPosition.xPosition + 1 + lb2D.blockField.size() * currentPosition.yPosition] == 0)
                {
                    nextField = Position2D{currentPosition.xPosition + 1,currentPosition.yPosition};
                    viableWayFound = true;
                }
                // possibility to go down
                if (lb2D.blockField[currentPosition.xPosition][currentPosition.yPosition + 1] == 0 &&
                 visitedFields[currentPosition.xPosition + lb2D.blockField.size() * (currentPosition.yPosition + 1)] == 0){
                    if (viableWayFound){
                        jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition});
                    }
                    else{
                        nextField = Position2D{currentPosition.xPosition,currentPosition.yPosition + 1};
                    }
                    viableWayFound = true;
                }
                // possibility to go  left
                if (lb2D.blockField[currentPosition.xPosition - 1][currentPosition.yPosition] == 0 
                && visitedFields[currentPosition.xPosition - 1 + lb2D.blockField.size() * currentPosition.yPosition] == 0){
                    if (viableWayFound){
                        jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition});
                    }
                    else{
                        nextField = Position2D{currentPosition.xPosition - 1,currentPosition.yPosition};
                    }
                    viableWayFound = true;
                }
                // go up
                if (lb2D.blockField[currentPosition.xPosition][currentPosition.yPosition - 1] == 0 
                && visitedFields[currentPosition.xPosition + lb2D.blockField.size() * (currentPosition.yPosition - 1)] == 0){
                    if (viableWayFound){
                        jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition});
                    }
                    else{
                        nextField = Position2D{currentPosition.xPosition,currentPosition.yPosition - 1};
                    }
                    viableWayFound = true;
                }
                if(!viableWayFound && jumpBackStack.size() > 0){ // go back to the last Position with a way
                    //currentPosition = jumpBackStack.front();
                    //jumpBackStack.erase((jumpBackStack.begin())); // safe first element and delete it from jump back stack
                    nextField = jumpBackStack.back();
                    jumpBackStack.pop_back();
                    // clear walked Postion (currentWayLength Position of the currentWayList)
                    // remove all position after the new current Position -> delete the way that reached dead end
                    auto foundPosition = std::find(currentWayList.begin(), currentWayList.end(), nextField);
                    currentWayList.erase(foundPosition + 1, currentWayList.end()); 
                    // foundPosition needs to be 1 bigger bc erase removes first element inclusive
                }
                // else if (jumpBackStack.size() == 0){
                //     return std::vector<Position2D>{}; // is this valid syntax?
                // }
                viableWayFound = false;
                visitedFields[currentPosition.xPosition + lb2D.blockField[0].size() * currentPosition.yPosition] = 1;
                currentWayList.push_back(nextField);
                currentPosition = nextField;
            }            
        }
        return currentWayList;
    }

/* works like findWay but doesn't delete old found free Positions and doesnt stop on the goal position */
    Way LabyrinthSolver::findConnectedTiles(const Labyrinth2D lb2D){
        std::vector<Position2D> currentWayList{lb2D.startPosition};
        std::vector<Position2D> jumpBackStack;
        Position2D currentPosition{lb2D.startPosition};

        std::vector<bool> visitedFields(lb2D.blockField.size() * lb2D.blockField[0].size());
        // worst case run time of n*m
        
        visitedFields[currentPosition.xPosition + lb2D.blockField.size() * currentPosition.yPosition] = 1;
        bool viableWayFound = false;
        for (auto vecIter = lb2D.blockField.begin(); vecIter != lb2D.blockField.end(); vecIter++)
        {
            for (auto bitIter = (*vecIter).begin(); bitIter != (*vecIter).end(); bitIter++)
            {
                //, 2.mark as visited,3. store jump back table if it still has free fields around
                //1. find next free field, that is not visited and free in the labyrinth
                // end if last Field is reached or there are no more ways
                viableWayFound = false;
                Position2D nextField{};
                // possibility to go right
                if (lb2D.blockField[currentPosition.xPosition + 1][currentPosition.yPosition] == 0 &&
                 visitedFields[currentPosition.xPosition + 1 + lb2D.blockField.size() * currentPosition.yPosition] == 0)
                {
                    nextField = Position2D{currentPosition.xPosition + 1,currentPosition.yPosition};
                    viableWayFound = true;
                }
                // possibility to go down
                if (lb2D.blockField[currentPosition.xPosition][currentPosition.yPosition + 1] == 0 &&
                 visitedFields[currentPosition.xPosition + lb2D.blockField.size() * (currentPosition.yPosition + 1)] == 0){
                    if (viableWayFound){
                        jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition + 1});
                    }
                    else{
                        nextField = Position2D{currentPosition.xPosition,currentPosition.yPosition + 1};
                    }
                    viableWayFound = true;
                }
                // possibility to go  left
                if (lb2D.blockField[currentPosition.xPosition - 1][currentPosition.yPosition] == 0 &&
                 visitedFields[currentPosition.xPosition - 1 + lb2D.blockField.size() * currentPosition.yPosition] == 0){
                    if (viableWayFound){
                        jumpBackStack.push_back(Position2D{currentPosition.xPosition - 1,currentPosition.yPosition});
                    }
                    else{
                        nextField = Position2D{currentPosition.xPosition - 1,currentPosition.yPosition};
                    }
                    viableWayFound = true;
                }
                // go up
                if (lb2D.blockField[currentPosition.xPosition][currentPosition.yPosition - 1] == 0 
                && visitedFields[currentPosition.xPosition + lb2D.blockField.size() * (currentPosition.yPosition - 1)] == 0){
                    if (viableWayFound){
                        jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition - 1});
                    }
                    else{
                        nextField = Position2D{currentPosition.xPosition,currentPosition.yPosition - 1};
                    }
                    viableWayFound = true;
                }
                if(!viableWayFound && jumpBackStack.size() == 0){
                    // the position got visited as last and still has to be added to the result!
                    currentWayList.push_back(currentPosition); 
                    return currentWayList;
                }

                if(!viableWayFound && jumpBackStack.size() > 0){ // go back to the last Position with a way
                    //currentPosition = jumpBackStack.front();
                    //jumpBackStack.erase((jumpBackStack.begin())); // safe first element and delete it from jump back stack
                    nextField = jumpBackStack.back();
                    jumpBackStack.pop_back();
                    // not necessary anymore, visited field are stored
                    // clear walked Postion (currentWayLength Position of the currentWayList)
                    // remove all position after the new current Position -> delete the way that reached dead end
                    //auto foundPosition = std::find(currentWayList.begin(), currentWayList.end(), nextField);
                    // foundPosition needs to be 1 bigger bc erase removes first element inclusive
                }
                viableWayFound = false;
                visitedFields[currentPosition.xPosition + lb2D.blockField.size() * currentPosition.yPosition] = 1;
                currentWayList.push_back(nextField);
                currentPosition = nextField;
            }            
        }
        return currentWayList;
    }

/*
*   expects all positions to be connected
* TODO solve more efficient by converting the way to a graph
*/
    bool LabyrinthSolver::hasLoops(const Way way){
        std::vector<Position2D> jumpBackStack;
        Position2D currentPosition = way[0];
        Position2D nextPosition;      
        //std::set<Position2D> visitedPositions{currentPosition}; // TODO maybe use a position to position to store taken
        std::map<Position2D,TakenPaths> pathsVisited;
        pathsVisited[currentPosition] = TakenPaths{false,false,false,false};

        // worst case run time of n*m
        bool nextPositionFound = false;
        bool jumpedBack = false;
        TakenPaths curPath = pathsVisited[currentPosition];
        for (auto vecIter = way.begin(); vecIter != way.end(); vecIter++){
            // find next position which is in the way object
            // take point with only one distance, if there is none
            nextPositionFound = false;
            TakenPaths newPath = TakenPaths{false,false,false,false};
            for (auto vecIter2 = way.begin();vecIter2 != way.end(); vecIter2++){
                // find all reachable Positions in the way, which are only visited once
                Position2D otherPosition = (*vecIter2);
                if (!(currentPosition == otherPosition) && PositionInterface::adjacentPoint(currentPosition, otherPosition) > 0){
                    std::cout << "from" << currentPosition << "to" << otherPosition <<"\n";
                    auto foundElementIt = pathsVisited.find(otherPosition);
                    if (foundElementIt == pathsVisited.end()){//element was not found in the map so add it with 1 appereance
                        curPath = pathsVisited[currentPosition]; // already existing path of the current Obj
                        newPath = takePath(currentPosition,otherPosition,newPath); // created path by going a direction
            
                        if (nextPositionFound){
                            jumpBackStack.push_back(otherPosition);
                        }
                        else{
                            nextPosition = otherPosition;
                            nextPositionFound = true;
                        }
                        pathsVisited[otherPosition] = mirrorPath(newPath); // the path back gets stored for the visited node
                        pathsVisited[currentPosition] = combinePaths(curPath, newPath); // the taken path is stored
                    }
                    else{
                        // we found a loop in the way if we come out on an already visitedPiece, from a different direction compared to before
                        // check if thats the piece we come from
                        //std::cout << "from" << currentPosition << "to" << otherPosition <<"\n";
                        std::cout << "the paths" << pathsVisited[currentPosition] << "\n";
                        //if (differentPathAlreadyTaken(currentPosition,otherPosition,pathsVisited[currentPosition])){ // not good for the jump back
                        
                        if (differentPathAlreadyTaken(currentPosition,otherPosition,pathsVisited[currentPosition]) && !jumpedBack ){
                            // we already came from a different position, -> looped
                                std::cout << "a different path was already taken\n";
                                return true;
                        }
                        else{
                            // mark the path as taken now
                            pathsVisited[otherPosition] = takePath(otherPosition, currentPosition,pathsVisited[otherPosition]);
                        }
                    }
                }
            }
            if (nextPositionFound){
                currentPosition = nextPosition;
                jumpedBack = false;
            } 
            else if(jumpBackStack.size() != 0){
                std::cout << "jumping back\n";
                jumpedBack = true;
                currentPosition = jumpBackStack.back();
                jumpBackStack.pop_back();
            }
            else {//if (jumpBackStack.size() == 0 && nextPositionFound == false){
                break;
            }
        }
        return false;
    }

    Way LabyrinthSolver::freeNeighbors(const BlockField2D field, const Position2D position,const int extendBorder){
        Way resultPositions;
        auto xPosition = position.xPosition;
        auto yPosition = position.yPosition;

        //TODO make extend border template or planning

        auto xLowerBorder = 0 + extendBorder;
        auto xUpperBorder = field.size() - 1 - extendBorder;
        auto yLowerBorder = 0 + extendBorder;
        auto yUpperBorder = field[0].size() - 1 - extendBorder;

        if (field[xPosition + 1][yPosition] == 1 && xPosition + 1 < xUpperBorder){
            resultPositions.push_back(Position2D{xPosition + 1,yPosition});
        }
        if (field[xPosition - 1][yPosition] == 1 && xPosition > xLowerBorder){
            resultPositions.push_back(Position2D{xPosition - 1,yPosition});
        }
        if (field[xPosition][yPosition + 1] == 1 && yPosition + 1 < yUpperBorder){
            resultPositions.push_back(Position2D{xPosition,yPosition + 1});           
        }
        if (field[xPosition][yPosition - 1] == 1 && yPosition > yLowerBorder){
            resultPositions.push_back(Position2D{xPosition,yPosition - 1});           
        }
        return resultPositions;
    }

// don't take pieces which create loops in an island
    Way LabyrinthSolver::freeNeighborsDifferentIsland(const BlockField2D field,const Way island, const Position2D position,const int extendBorder){
        Way resultPositions;
        auto xPosition = position.xPosition;
        auto yPosition = position.yPosition;

        //TODO make extend border template or planning

        auto xLowerBorder = 0 + extendBorder;
        auto xUpperBorder = field.size() - 1 - extendBorder;
        auto yLowerBorder = 0 + extendBorder;
        auto yUpperBorder = field[0].size() - 1 - extendBorder;

        const Position2D positiveX{xPosition + 1,yPosition};
        const Position2D negativeX{xPosition - 1,yPosition};
        const Position2D positiveY{xPosition, yPosition + 1};
        const Position2D negativeY{xPosition, yPosition - 1};
        if (field[xPosition + 1][yPosition] == 1 && xPosition + 1 < xUpperBorder && (PositionInterface::timesAdjacentToWay(positiveX,island) == 1)){
            resultPositions.push_back(positiveX);
        }
        if (field[xPosition - 1][yPosition] == 1 && xPosition > xLowerBorder && (PositionInterface::timesAdjacentToWay(negativeX,island) == 1)){
            resultPositions.push_back(negativeX);
        }
        if (field[xPosition][yPosition + 1] == 1 && yPosition + 1 < yUpperBorder && (PositionInterface::timesAdjacentToWay(positiveY,island) == 1)){
            resultPositions.push_back(positiveY);
        }
        if (field[xPosition][yPosition - 1] == 1 && yPosition > yLowerBorder && (PositionInterface::timesAdjacentToWay(negativeY,island) == 1)){
            resultPositions.push_back(negativeY);
        }
        return resultPositions;
    }

    Way LabyrinthSolver::newCombinedIsland(const BlockField2D field,const Way island, const Position2D position,const int extendBorder){
        Way resultPositions;
        auto xPosition = position.xPosition;
        auto yPosition = position.yPosition;

        //TODO make extend border template or planning

        auto xLowerBorder = 0 + extendBorder;
        auto xUpperBorder = field.size() - 1 - extendBorder;
        auto yLowerBorder = 0 + extendBorder;
        auto yUpperBorder = field[0].size() - 1 - extendBorder;
        std::cout << "upper border" << yUpperBorder << field[xPosition][yPosition + 1];

        const Position2D positiveX{xPosition + 1,yPosition};
        const Position2D negativeX{xPosition - 1,yPosition};
        const Position2D positiveY{xPosition, yPosition + 1};
        const Position2D negativeY{xPosition, yPosition - 1};
        if (field[xPosition + 1][yPosition] == 1 && xPosition + 1 < xUpperBorder && (PositionInterface::timesAdjacentToWay(positiveX,island) == 1)){
            resultPositions.push_back(positiveX);
        }
        if (field[xPosition - 1][yPosition] == 1 && xPosition > xLowerBorder && (PositionInterface::timesAdjacentToWay(negativeX,island) == 1)){
            resultPositions.push_back(negativeX);
        }
        if (field[xPosition][yPosition + 1] == 1 && yPosition + 1 < yUpperBorder && (PositionInterface::timesAdjacentToWay(positiveY,island) == 1)){
            resultPositions.push_back(positiveY);
        }
        if (field[xPosition][yPosition - 1] == 1 && yPosition > yLowerBorder && (PositionInterface::timesAdjacentToWay(negativeY,island) == 1)){
            resultPositions.push_back(negativeY);
        }
        return resultPositions;
    }


// expects all ways to be connected
// find all the Positions which contribute to a a looped way, Problem the way could have multiple loops...
// what to do then, "just" merge them
// idea1: use edge pieces to determine where a piece belongs?
// Way LabyrinthSolver::findLoopPositions(const Way way){


//         Position2D currentPosition = way[0];
//         Position2D nextPosition;
        
//         std::map<Position2D,int> timesVisited;
        
//         timesVisited[currentPosition] = 1;
//         // worst case run time of n*m
//         bool foundLoop = false;
//         for (auto vecIter = way.begin(); vecIter != way.end(); vecIter++) //TODO maybe sort
//         {
//             if (foundLoop){
//                 break;
//             }
//             // find next position which is in the way object
//             // take point with only one distance, if there is none
//             nextPositionFound = false;
//             for (auto vecIter2 = way.begin();vecIter2 != way.end(); vecIter2++){
//                 if (!(currentPosition == (*vecIter2))){
//                     // find all reachable Positions in the way, which are only visited once
//                     if (PositionInterface::adjacentPoint(currentPosition, *vecIter2) > 0){
//                         auto foundElementIt = timesVisited.find(*vecIter2);
//                         // element was never visited
//                         if (foundElementIt == timesVisited.end()){ // element was not found in the map so add it with 1 appereance
//                             timesVisited[(*vecIter2)] = 1;
//                             currentPosition = (*vecIter2);
//                             break;
//                         }
//                         else{
//                             // element was visited once or twice
//                             // if the element was visited twice already we have a loop
//                             if (timesVisited[*vecIter2] == 1){
//                                 timesVisited[(*vecIter2)] = 2;
//                             }
//                             else{
//                                 //element was already over the back position -> with two times visited a loop exists
//                                 timesVisited[currentPosition] = 2;
//                                 foundLoop = true;
//                                 break;
//                             }
//                         }
                        
//                     }
//                 }
//             }
//         }     
//         Way resultWay; // use all the nodes which were visited twice and have two adjacent PositionObjects
//         return false;
//     }


    Way LabyrinthSolver::findConnectedTilesSet(const Labyrinth2D lb2D){
        Way connectedTiles = LabyrinthSolver::findConnectedTiles(lb2D);
        std::set<Position2D> convSet(connectedTiles.begin(), connectedTiles.end());
        Way resultWay(convSet.begin(), convSet.end());
        return resultWay;
        
    }

    Way LabyrinthSolver::findConnectedTilesSet(const BlockField2D blockField, Position2D startPosition){
        Way connectedTiles = LabyrinthSolver::findConnectedTiles(Labyrinth2D{startPosition,startPosition,blockField});
        std::set<Position2D> convSet(connectedTiles.begin(), connectedTiles.end());
        Way resultWay(convSet.begin(), convSet.end());
        return resultWay;
        
    }

    
    WaysVector LabyrinthSolver::findAllWays(const Labyrinth2D lb2D){
        WaysVector result;
        //const Labyrinth2D constCopy = lb2D; // gives the iterator that shouldnt be changed in the loop -> ugly!
        Labyrinth2D copiedLabyrinth = lb2D; // the labyrinth that will be edited
        auto xSize = copiedLabyrinth.blockField.size();
        auto ySize = copiedLabyrinth.blockField[0].size();
        BlockField2D curBlockField = copiedLabyrinth.blockField; // the blockfield that will be edited
        Position2D curStartPosition{1,1};//copiedLabyrinth.startPosition;
        // for (auto vecIter =(constCopy.blockField).begin(); vecIter != (constCopy.blockField).end(); vecIter++)
        // {
        for (auto indexXCoord = 1; indexXCoord <= xSize - 2; indexXCoord++)
        {
            for (auto indexYCoord = 1; indexYCoord <= ySize - 2; indexYCoord++)
            {
            // auto indexXCoord = vecIter - (constCopy.blockField).begin();
            // for (auto bitIter = (*vecIter).begin(); bitIter != (*vecIter).end(); bitIter++)
            // {
                // auto indexYCoord = bitIter - (*vecIter).begin()++;
                if (curBlockField[indexXCoord][indexYCoord] == 0){ // free field
                    curStartPosition = Position2D{indexXCoord, indexYCoord};
                    copiedLabyrinth.startPosition = curStartPosition;
                    Way currentWay = findConnectedTilesSet(copiedLabyrinth);
                    // std::cout << "fill points:\n";
                    // for (const Position2D pos : currentWay){
                    //     std::cout << pos << "\t";
                    // }
                    // std::cout <<"\n";
                    curBlockField = Lab::Labyrinth2DGenerator::fillPoints(curBlockField, currentWay, 1); // fill found fields
                    copiedLabyrinth.blockField = curBlockField;
                    result.push_back(currentWay);
                }
            }
        }
        return result;
    }

    WaysVector LabyrinthSolver::findAllWays(const BlockField2D blockField){
        WaysVector result;
        BlockField2D curBlockField = blockField;// the blockfield that will be edited
        Position2D curStartPosition{1,1};//copiedLabyrinth.startPosition;
        // for (auto vecIter =(constCopy.blockField).begin(); vecIter != (constCopy.blockField).end(); vecIter++)
        // {
        for (auto indexXCoord = 1; indexXCoord <= curBlockField.size() - 2; indexXCoord++)
        {
            for (auto indexYCoord = 1; indexYCoord <= curBlockField.size() - 2; indexYCoord++)
            {
            // auto indexXCoord = vecIter - (constCopy.blockField).begin();
            // for (auto bitIter = (*vecIter).begin(); bitIter != (*vecIter).end(); bitIter++)
            // {
                // auto indexYCoord = bitIter - (*vecIter).begin()++;
                if (curBlockField[indexXCoord][indexYCoord] == 0){ // free field
                    curStartPosition = Position2D{indexXCoord, indexYCoord};
                    Way currentWay = findConnectedTilesSet(curBlockField,curStartPosition);
                    curBlockField = Lab::Labyrinth2DGenerator::fillPoints(curBlockField, currentWay, 1); // fill found fields
                    result.push_back(currentWay);
                }
            }
        }
        return result;
    }

    Labyrinth2D Labyrinth2DGenerator::depthSearchLabyrinth(const Labyrinth2D lab2d){
        // find all Ways
        Labyrinth2D copiedLabyrinth = lab2d;
        BlockField2D currentBlockField = lab2d.blockField;
        WaysVector allWays = LabyrinthSolver::findAllWays(lab2d);
        std::set<Position2D> allPoints;
        std::set<Position2D> coveredPoints;
        for (Way way : allWays){
            allPoints.insert(way.begin(), way.end());
        }
        Position2D currentPosition = Position2D{1,1};
        Position2D nextPosition{-1,-1}; // use invalid Position to mark not found new Position
        // go over points that are reacheable safe BackJumpStack for deadends
        std::vector<Position2D> jumpBackStack;
        // maximum of n*n steps
        for (size_t currentRun = 0; currentRun < currentBlockField.size()*currentBlockField[0].size(); currentRun++){
            // find free nextFiled
            nextPosition = Position2D{-1,-1};
            if (currentBlockField[currentPosition.xPosition + 1][currentPosition.yPosition] == 0){
                nextPosition = Position2D{currentPosition.xPosition + 1,currentPosition.yPosition};
            }
            if (currentBlockField[currentPosition.xPosition - 1][currentPosition.yPosition] == 0){
                if (nextPosition.xPosition == -1){
                    jumpBackStack.push_back(Position2D{currentPosition.xPosition - 1,currentPosition.yPosition});
                }
                else {
                    nextPosition = Position2D{currentPosition.xPosition - 1,currentPosition.yPosition};
                }
            }
            if (currentBlockField[currentPosition.xPosition][currentPosition.yPosition + 1] == 0){
                if (nextPosition.xPosition == -1){
                    jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition + 1});
                }
                else {
                    nextPosition = Position2D{currentPosition.xPosition,currentPosition.yPosition + 1};
                }
            }
            if (currentBlockField[currentPosition.xPosition][currentPosition.yPosition - 1] == 0){
                if (nextPosition.xPosition == -1){
                    jumpBackStack.push_back(Position2D{currentPosition.xPosition,currentPosition.yPosition - 1});
                }
                else {
                    nextPosition = Position2D{currentPosition.xPosition,currentPosition.yPosition -1};
                }
            }
            if (nextPosition.xPosition == -1 && jumpBackStack.size() == 0){
                // free a "random" field
                // TODO continue here
                //currentPosition;
            }
            coveredPoints.insert(currentPosition);


        }
        copiedLabyrinth.blockField = currentBlockField;
        return copiedLabyrinth;
    }


    // Template instantiations
    template void PrintInterface::print2DContainer(const std::vector<std::vector<bool>> &inputVector);
    template void PrintInterface::print2DContainer(const std::vector<std::vector<int>> &inputVector);
}


/*
                if (indexXCoord == 1 && indexYCoord == 5){
                    for (Position2D pos : currentWay){
                        std::cout << "x" << pos.xPosition << " y" << pos.yPosition << "\n";
                    }
                }
*/